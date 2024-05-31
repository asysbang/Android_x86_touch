package com.asysbang.touch;

import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        copyAssets();

    }

    private void copyAssets() {
        try {
            File dir = getFilesDir();
            Log.e("", "======getFilesDir=" + dir.getAbsolutePath());
            File karlFile = new File(dir.getAbsolutePath() + "/karl");
            if (karlFile.exists()) {
                Log.e("", "======exists=11");
                startMainService();
                return;
            }
            AssetManager assetManager = getAssets();
            InputStream inputStream = assetManager.open("karl");
            FileOutputStream fos = new FileOutputStream(dir.getAbsolutePath() + "/karl");
            byte[] buf = new byte[2048];
            int len = -1;
            while ((len = inputStream.read(buf)) != -1) {
                fos.write(buf, 0, len);
            }
            fos.flush();
            fos.close();
            inputStream.close();

            inputStream = assetManager.open("run.sh");
            fos = new FileOutputStream(dir.getAbsolutePath() + "/run.sh");
            while ((len = inputStream.read(buf)) != -1) {
                fos.write(buf, 0, len);
            }
            fos.flush();
            fos.close();
            inputStream.close();
            startMainService();

        } catch (IOException e) {
            throw new RuntimeException(e);
        }


    }

    private void startMainService() {
        startService(new Intent(this, MainService.class));
    }

    //    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(MainActivity.this) {
//        @Override
//        public void onManagerConnected(int status) {
//            super.onManagerConnected(status);
//            Log.e("", "========onManagerConnected=" + status);
//        }
//    };
    @Override
    protected void onResume() {
        super.onResume();
//        System.load(Core.NATIVE_LIBRARY_NAME);
//        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
//        OpenCVNativeLoader nativeLoader = new OpenCVNativeLoader();
//        nativeLoader.init();
        //OpenCV.loadShared();
//        if (!OpenCVLoader.initDebug()) {
//            Log.d("aaa", "===Internal OpenCV library not found. Using OpenCV Manager for initialization");
//            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, MainActivity.this, mLoaderCallback);
//        } else {
//            Log.d("", "=====OpenCV library found inside package. Using it!");
//            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
//        }
    }
}