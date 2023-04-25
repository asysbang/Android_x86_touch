/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <binder/ProcessState.h>

#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>

#include <ui/DisplayInfo.h>
#include <ui/PixelFormat.h>

#include <system/graphics.h>

#include <cutils/sockets.h>

// TODO: Fix Skia.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <SkImageEncoder.h>
#include <SkData.h>
#include <SkColorSpace.h>
#pragma GCC diagnostic pop

using namespace android;

static uint32_t DEFAULT_DISPLAY_ID = ISurfaceComposer::eDisplayIdMain;

#define COLORSPACE_UNKNOWN    0
#define COLORSPACE_SRGB       1
#define COLORSPACE_DISPLAY_P3 2

int main() {

	int32_t displayId = DEFAULT_DISPLAY_ID;
	fprintf(stderr, "==========================karl main\n");
	int fd = -1;

	void const* mapbase = MAP_FAILED;
	ssize_t mapsize = -1;

	void* base = NULL;
	uint32_t w, s, h, f;
	android_dataspace d;
	size_t size = 0;

	// Maps orientations from DisplayInfo to ISurfaceComposer
	static const uint32_t ORIENTATION_MAP[] = { ISurfaceComposer::eRotateNone, // 0 == DISPLAY_ORIENTATION_0
			ISurfaceComposer::eRotate270, // 1 == DISPLAY_ORIENTATION_90
			ISurfaceComposer::eRotate180, // 2 == DISPLAY_ORIENTATION_180
			ISurfaceComposer::eRotate90, // 3 == DISPLAY_ORIENTATION_270
			};

	// setThreadPoolMaxThreadCount(0) actually tells the kernel it's
	// not allowed to spawn any additional threads, but we still spawn
	// a binder thread from userspace when we call startThreadPool().
	// See b/36066697 for rationale
	ProcessState::self()->setThreadPoolMaxThreadCount(0);
	ProcessState::self()->startThreadPool();

	sp < IBinder > display = SurfaceComposerClient::getBuiltInDisplay(
			displayId);
	if (display == NULL) {
		fprintf(stderr, "Unable to get handle for display %d\n", displayId);
		// b/36066697: Avoid running static destructors.
		_exit(1);
	}

	Vector < DisplayInfo > configs;
	SurfaceComposerClient::getDisplayConfigs(display, &configs);
	int activeConfig = SurfaceComposerClient::getActiveConfig(display);
	if (static_cast<size_t>(activeConfig) >= configs.size()) {
		fprintf(stderr, "Active config %d not inside configs (size %zu)\n",
				activeConfig, configs.size());
		// b/36066697: Avoid running static destructors.
		_exit(1);
	}
	uint8_t displayOrientation = configs[activeConfig].orientation;
	uint32_t captureOrientation = ORIENTATION_MAP[displayOrientation];

	sp < GraphicBuffer > outBuffer;
	status_t result = ScreenshotClient::capture(display, Rect(),
			0 /* reqWidth */, 0 /* reqHeight */, INT32_MIN, INT32_MAX, /* all layers */
			false, captureOrientation, &outBuffer);
	if (result != NO_ERROR) {
		close(fd);
		_exit(1);
	}

	result = outBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN, &base);

	if (base == NULL) {
		close(fd);
		_exit(1);
	}

	w = outBuffer->getWidth();
	h = outBuffer->getHeight();
	s = outBuffer->getStride();
	f = outBuffer->getPixelFormat();
	d = HAL_DATASPACE_UNKNOWN;
	size = s * h * bytesPerPixel(f);

	fprintf(stderr, "==========================getWidth = %d\n", w);
	close(fd);
	if (mapbase != MAP_FAILED) {
		munmap((void *) mapbase, mapsize);
	}
//socket
	int serverFd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (-1 == serverFd) {
		fprintf(stderr, "==========================serverFd = -1\n");
		return -1;
	}

	sockaddr_un serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	strcpy(serveraddr.sun_path + 1, "/tmp/mysocket");
	serveraddr.sun_family = AF_LOCAL;
	socklen_t addrlen = 1 + strlen("/tmp/mysocket")
			+ sizeof(serveraddr.sun_family);
	int ret = bind(serverFd, (struct sockaddr *) &serveraddr, addrlen);
	if (-1 == ret) {
		fprintf(stderr, "==========================ret = -1\n");
		return -1;
	}

	ret = listen(serverFd, 100);
	if (-1 == ret) {
		fprintf(stderr, "==========================listen = -1\n");
		return -1;
	}

	int clientfd = -1;
	int read_len = -1;
	char buf[100];
	while (1) {
		clientfd = accept(serverFd, (struct sockaddr *) NULL, NULL);
		if (-1 == clientfd) {
			fprintf(stderr, "==========================accept = -1\n");
			continue;
		}
		fprintf(stderr, "==========================new accept \n");
		while (clientfd) {
			read_len = read(clientfd, &buf, sizeof(buf));
			if (read_len > 0) {
				fprintf(stderr, "=========================server read %d, %s\n",
						read_len, buf);
				const char* msg = "server send world\n";
				write(clientfd, msg, strlen(msg));
			} else {
				signal(SIGPIPE, SIG_IGN);
				break;
			}
		}
	}
}

