package com.asysbang.touch;

import android.app.Service;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.os.IBinder;
import android.util.Log;

import com.asysbang.touch.su.SuHelper;

public class MainService extends Service {

    private static final String TAG = "MainService";

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }

    private void runTestClient() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                NativeHelper mHelper = new NativeHelper();
                int connected = mHelper.connectServer();
                if (connected == 1) {
                    Log.e(TAG, "=======runTestClient getHeight1 : " + mHelper.getHeight());
                    Log.e(TAG, "=======runTestClient getWidth1 : " + mHelper.getWidth());
                    //Log.e(TAG, "=======runTestClient getRgb1 : " + mHelper.getRgb(53,53));
                    Log.e(TAG, "=======png start: ");
                    int png = mHelper.getNewPng();
                    Log.e(TAG, "=======png : " + png);
                } else {
                    Log.e(TAG, "=======connectServer failed : " + connected);
                }
                Log.e(TAG, "=======over: ");
            }
        }).start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.e(TAG, "=======onStartCommand:"+getFilesDir().getAbsolutePath());
        SuHelper.getInstance().runSu();
        //SuHelper.getInstance().getScreencap();
        SuHelper.getInstance().runCmd("sh " + getFilesDir().getAbsolutePath() + "/run.sh\n");
        runTestClient();
        return super.onStartCommand(intent, flags, startId);
    }
}
