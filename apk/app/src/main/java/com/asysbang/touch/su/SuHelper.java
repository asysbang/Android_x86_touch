package com.asysbang.touch.su;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class SuHelper {

    private SuHelper() {

    }

    private static SuHelper sInstance;

    public static SuHelper getInstance() {
        if (null == sInstance) {
            sInstance = new SuHelper();
        }
        return sInstance;
    }

    private Process mSuProcess;

    private BufferedOutputStream mBos;

    public void runCmd(String cmd) {

        try {
            byte[] cmdBytes = cmd.getBytes();
            mBos.write(cmdBytes, 0, cmdBytes.length);
            mBos.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

    }

    public void runSu() {
        Runtime runtime = Runtime.getRuntime();
        try {
            mSuProcess = runtime.exec("su");
            mBos = (BufferedOutputStream) mSuProcess.getOutputStream();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
