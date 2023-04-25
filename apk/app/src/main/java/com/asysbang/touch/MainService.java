package com.asysbang.touch;

import android.app.Service;
import android.content.Intent;
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

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.e(TAG,"=======onStartCommand");
        SuHelper.getInstance().runSu();
        SuHelper.getInstance().runCmd("sh "+getFilesDir().getAbsolutePath()+"/run.sh\n");
        return super.onStartCommand(intent, flags, startId);
    }
}
