package com.asysbang.touch;

public class NativeHelper {

    static {
       System.loadLibrary("touch");
    }

    native int connectServer();

    native int getWidth();

    native int getHeight();

    native int getPng();

    native int getRgb(int x ,int y);
}
