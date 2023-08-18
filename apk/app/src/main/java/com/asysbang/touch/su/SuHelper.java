package com.asysbang.touch.su;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;

public class SuHelper {

    private static SuHelper sInstance;
    private Process mSuProcess;
    private BufferedOutputStream mBos;
    private BufferedInputStream mBis;

    private SuHelper() {

    }

    public static SuHelper getInstance() {
        if (null == sInstance) {
            sInstance = new SuHelper();
        }
        return sInstance;
    }

    public void runCmd(String cmd) {
        try {
            byte[] cmdBytes = cmd.getBytes();
            mBos.write(cmdBytes, 0, cmdBytes.length);
            mBos.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void getScreencap() {
        try {
            Log.e("", "==========start");
            String cmd = "screencap -p /data/user/0/com.asysbang.touch/files/xx.png\n";
            byte[] cmdBytes = cmd.getBytes();
            mBos.write(cmdBytes, 0, cmdBytes.length);

            String cmd1 = "echo $?\n";
            byte[] cmd1Bytes = cmd1.getBytes();
            mBos.write(cmd1Bytes, 0, cmd1Bytes.length);
            mBos.flush();
            byte[] buf = new byte[128];
            int len = -1;
            while ((len = mBis.read(buf, 0, 14)) != -1) {
                Log.e("", "==========" + len);
                break;
            }
//            String cmd2 = "chmod 777 /data/user/0/com.asysbang.touch/files/xx.png\n";
//            byte[] cmd2Bytes = cmd2.getBytes();
//            mBos.write(cmd2Bytes, 0, cmd2Bytes.length);
//            mBos.flush();
//            Thread.sleep(30);
            Bitmap bitmap = BitmapFactory.decodeFile("/data/user/0/com.asysbang.touch/files/xx.png");

            Log.e("", "==========over" + bitmap.getHeight());
        } catch (Exception e) {

        }

    }

    public void runSu() {
        Runtime runtime = Runtime.getRuntime();
        try {
            mSuProcess = runtime.exec("su");
            mBos = (BufferedOutputStream) mSuProcess.getOutputStream();
            mBis = (BufferedInputStream) mSuProcess.getInputStream();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
