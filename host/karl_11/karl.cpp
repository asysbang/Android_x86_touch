#include <karl.h>
#include <binder/ProcessState.h>
#include <sys/mman.h>
#include <sys/un.h>
#include <android/log.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <ui/DisplayInfo.h>
using namespace android;
uint32_t f;

  

//获取截图数据，尝试mmap到指定文件  
sp <GraphicBuffer> getScreencap() {
  LOGE("======>>>getScreencap\n");
  fprintf(stderr, "======>>>getScreencap\n");
  //void *base1 = NULL;
  ProcessState::self()->setThreadPoolMaxThreadCount(0);
  ProcessState::self()->startThreadPool();
    std::optional<PhysicalDisplayId> displayId = SurfaceComposerClient::getInternalDisplayId();
    if (!displayId) {
        fprintf(stderr, "Failed to get token for internal display\n");
         _exit(1);
    }
    ui::Dataspace outDataspace;
    sp<GraphicBuffer> outBuffer;

    status_t result = ScreenshotClient::capture(*displayId, &outDataspace, &outBuffer);
  if (result != NO_ERROR) {
    _exit(1);
  }
  
/**  
  result = outBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN, &base1);
  if (base1 == NULL) {
    _exit(1);
  }

  screen_width= outBuffer->getWidth();
  screen_height = outBuffer->getHeight();
  screen_stride = outBuffer->getStride();
  f = outBuffer->getPixelFormat();
  size = screen_stride * screen_height * bytesPerPixel(f);
  
unsigned char *rgb = (unsigned char*) base1 + (53 * screen_stride + 53) * 4;
fprintf(stderr, "======getScreencap>>>color %d,%d,%d,%d,%d,%d,%d,%d,%d\n",rgb[0],rgb[1],rgb[2],rgb[3],rgb[4],rgb[5],rgb[6],rgb[7],rgb[8]);
  
  
  //开始尝试mmap数据
  void const* mapbase = MAP_FAILED;
  ssize_t mapsize = -1;
  
  int fd = open("/tmp/mmaptem",O_RDWR|O_CREAT|O_TRUNC,0666);
  
	mapsize =  size;
	mapbase = mmap(0, mapsize, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mapbase != MAP_FAILED) {
		 fprintf(stderr, "======>>>mapbase ok,\n");
		 //mapbase = base;//怎么把base的数据映射到mapbase上？？？？
	} else {
		 fprintf(stderr, "======>>>mapbase error\n");
	}
  */
  return outBuffer;
}


int main() {
  LOGE("======>>>karl service started ! ! ! \n");
  void *base = NULL;
  if(screen_width <= 0 ){
   getScreencap();
   }
// setThreadPoolMaxThreadCount(0) actually tells the kernel it's
// not allowed to spawn any additional threads, but we still spawn
// a binder thread from userspace when we call startThreadPool().
// See b/36066697 for rationale
ProcessState::self()->setThreadPoolMaxThreadCount(0);
ProcessState::self()->startThreadPool();
    std::optional<PhysicalDisplayId> displayId = SurfaceComposerClient::getInternalDisplayId();
    if (!displayId) {
        fprintf(stderr, "Failed to get token for internal display\n");
        return 1;
    }
    ui::Dataspace outDataspace;
    sp<GraphicBuffer> outBuffer;

    status_t result = ScreenshotClient::capture(*displayId, &outDataspace, &outBuffer);
if (result != NO_ERROR) {
	_exit(1);
}

result = outBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN, &base);

if (base == NULL) {
	_exit(1);
}
screen_width= outBuffer->getWidth();
screen_height = outBuffer->getHeight();
screen_stride = outBuffer->getStride();
f = outBuffer->getPixelFormat();

size = screen_stride * screen_height * bytesPerPixel(f);

size_t Bpp = bytesPerPixel(f);
//for (size_t y=0 ; y<h ; y++) {
//    write(fd, base, w*Bpp);
//    base = (void *)((char *)base + s*Bpp);
//}
fprintf(stderr, "======>>>size %ld\n",size);
fprintf(stderr, "======>>>s %u\n",screen_stride);
fprintf(stderr, "======>>>Bpp %ld\n",Bpp);
unsigned char *rgb = (unsigned char*) base + (53 * screen_stride + 53) * 4;
fprintf(stderr, "======>>>color %d,%d,%d,%d,%d,%d,%d,%d,%d\n",rgb[0],rgb[1],rgb[2],rgb[3],rgb[4],rgb[5],rgb[6],rgb[7],rgb[8]);

//socket
int serverFd = socket(AF_LOCAL, SOCK_STREAM, 0);
if (-1 == serverFd) {
	LOGE("======>>>serverFd = -1\n");
	return -1;
}

sockaddr_un serveraddr;
bzero(&serveraddr, sizeof(serveraddr));
strcpy(serveraddr.sun_path + 1, "/data/user/0/com.asysbang.touch/files/mysocket");
serveraddr.sun_family = AF_LOCAL;
socklen_t addrlen = 1 + strlen("/data/user/0/com.asysbang.touch/files/mysocket") + sizeof(serveraddr.sun_family);
int ret = bind(serverFd, (struct sockaddr*) &serveraddr, addrlen);
if (-1 == ret) {
	LOGE("======>>>ret = -1\n");
	return -1;
}

ret = listen(serverFd, 100);
if (-1 == ret) {
	fprintf(stderr, "======>>>listen = -1\n");
	return -1;
}

int clientfd = -1;
int read_len = -1;
char buf[4];
while (1) {
	clientfd = accept(serverFd, (struct sockaddr *) NULL, NULL);
	if (-1 == clientfd) {
		fprintf(stderr, "======>>>accept = -1\n");
		continue;
	}
	fprintf(stderr, "======>>>new accept come\n");
	while (clientfd) {
		read_len = read(clientfd, &buf, sizeof(buf));
		if (read_len > 0) {
//			buf[read_len] = '\0';
			fprintf(stderr, "======>>>server read %d, %s\n", read_len, buf);
			LOGE("======>>>server read=  %d , %s\n",read_len, buf);
			//const char* msg = "server send world\n";
			if(buf[0] == 'w') {
				fprintf(stderr, "======>>>receive width request= %d\n",screen_width);
				LOGE("======>>>receive width request=  %d\n",screen_width);
				write(clientfd, &screen_width, sizeof(screen_width));
			} else if (buf[0] == 'h') {
				fprintf(stderr, "======>>>receive height request = %d\n",screen_height);
				LOGE("======>>>receive height request=  %d\n",screen_height);
				write(clientfd, &screen_height, sizeof(screen_height));
			} else if (buf[0] == 'r') {
				int x;
				read(clientfd, &x, sizeof(x));
				int y;
				read(clientfd, &y, sizeof(y));
				fprintf(stderr, "======>>>receive rgb request , poi is [%d,%d]\n",x,y);
				 LOGE("======>>>receive rgb request , poi is [%d,%d]\n",x,y);
				sp <GraphicBuffer> newOutBuffer = getScreencap();
				
				
				status_t result1 = newOutBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN, &screen_base);
				  if (result1 != NO_ERROR) {
				    _exit(1);
				  }

				unsigned char * rgb = (unsigned char *)screen_base+(y*screen_stride+x)*4;
				fprintf(stderr, "======>>>color1111 %d,%d,%d,%d,%d,%d,%d,%d,%d\n",rgb[0],rgb[1],rgb[2],rgb[3],rgb[4],rgb[5],rgb[6],rgb[7],rgb[8]);
				write(clientfd, &rgb[0], sizeof(rgb[0]));
				write(clientfd, &rgb[1], sizeof(rgb[1]));
				write(clientfd, &rgb[2], sizeof(rgb[2]));
								fprintf(stderr, "======>>> \n");
								LOGE("======>>>server send rgb size %ld,%ld,%ld\n",sizeof(rgb[0]),sizeof(rgb[1]),sizeof(rgb[2]));
			} 
			else if (buf[0] == 'p') {
				sp <GraphicBuffer> newOutBuffer = getScreencap();
				status_t result1 = newOutBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN, &screen_base);
				  if (result1 != NO_ERROR) {
				    _exit(1);
				  }
				  //unsigned char * rgb = (unsigned char *)screen_base;
				  fprintf(stderr, "receive png request , size is %ld\n",size);
				  LOGE("======>>>receive png request , size is %ld\n",size);
				  size_t line = 0;
				  for(size_t y=0;y<screen_height;) {
				  
  				    size_t d= 100;
  				    line = line +100;
  				    if(line > screen_height) {
  				      line = line -100;
  				      d = screen_height - line;
  				    }
  				    y=y+100;
  				    size_t sendSize = screen_width*4*d;
  				   // fprintf(stderr, "======>>>receive png request , send lengt1111h = %ld\n",sendSize);
  				    //fprintf(stderr, "======>>>receive png request , 11111\n");
  				    write(clientfd, screen_base, sendSize);//1024 *4
    				   LOGE("======>>>receive png request , send length = %ld\n",sendSize);
				   fprintf(stderr, "======>>>receive png request , send length = %ld\n",sendSize);
  				   screen_base = (void *)((char *)screen_base + sendSize);
				  }
				  

				   LOGE("======>>>receive png request , send over\n");
				   fprintf(stderr, "======>>>receive png request , send over\n");
			}
			else {
				fprintf(stderr, "======>>>strcmp else\n");
				write(clientfd, &read_len, sizeof(read_len));
			}
		} else {
			signal(SIGPIPE, SIG_IGN);
			break;
		}
	}
}
}


