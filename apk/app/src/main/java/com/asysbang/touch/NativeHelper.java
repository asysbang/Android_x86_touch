package com.asysbang.touch;

public class NativeHelper {

    static {
        System.loadLibrary("touch");
    }

    native int[] testBitmap(int[] pix, int width, int height);

    native int connectServer();

    native int getWidth();

    native int getHeight();

    native char[] getRgb(int x, int y);//暂时不可用，因为返回的char不能和java中的char对应


    native int getNewPng();

    native int[] getBitmapPixels(int mScreenWidth, int mScreenHeight);
}
