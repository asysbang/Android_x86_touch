package com.asysbang.touch;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import com.asysbang.touch.su.SuHelper;

import java.lang.annotation.Native;

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
                Log.e(TAG,"=======runTestClient new : "+mHelper.getWidth());
            }
        }).start();

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.e(TAG,"=======onStartCommand");
        SuHelper.getInstance().runSu();
        SuHelper.getInstance().runCmd("sh "+getFilesDir().getAbsolutePath()+"/run.sh\n");
        runTestClient();
        return super.onStartCommand(intent, flags, startId);
    }
}
