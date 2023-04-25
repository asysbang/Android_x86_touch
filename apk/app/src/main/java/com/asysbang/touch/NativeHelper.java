package com.asysbang.touch;

public class NativeHelper {

    static {
       System.loadLibrary("touch");
    }

    native int getWidth();
}
