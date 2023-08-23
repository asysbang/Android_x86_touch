package com.asysbang.touch;

import android.app.Service;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.WindowManager;
import android.widget.Toast;

import com.asysbang.touch.su.SuHelper;
import com.asysbang.touch.ui.FloatView;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;

public class MainService extends Service implements FloatView.FloatViewListener {

    private static final String TAG = "MainService";

    private WindowManager mWindowManager;
    private WindowManager.LayoutParams mLp;
    private FloatView mFloatView;

    private NativeHelper mNativeHelper;

    private int mIsConnected = -1;

    private int mScreenWidth, mScreenHeight;

    private void addFloatWindow() {
        mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
        mFloatView = new FloatView(this, this);
        mLp = new WindowManager.LayoutParams(WindowManager.LayoutParams.WRAP_CONTENT, WindowManager.LayoutParams.WRAP_CONTENT);
        mLp.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;
        mLp.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        mLp.gravity = Gravity.TOP | Gravity.LEFT;
        mLp.x = 100;
        mLp.y = 100;
        mLp.width = 380;
        mLp.height = 80;
        try {
            mWindowManager.addView(mFloatView, mLp);
        } catch (Exception e){
            e.printStackTrace();
            Toast.makeText(getApplicationContext(),"please allow this app show on other apps in settings",Toast.LENGTH_LONG).show();
        }

    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        mNativeHelper = new NativeHelper();
        addFloatWindow();
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
                mIsConnected = mNativeHelper.connectServer();
                if (mIsConnected == 1) {
                    int width = mNativeHelper.getWidth();
                    int height = mNativeHelper.getHeight();
                    Log.e(TAG, "=======runTestClient width is " + width + " , height is " + height);
//                    char[] rgb = mHelper.getRgb(53, 53);//????????????? 应该是byte 还是char
//                    Log.e(TAG, "=======runTestClient rgb : " + (int) rgb[0] + " , " + (int) rgb[1] + " , " + (int) rgb[2]);
////                    Log.e(TAG, "=======png start: ");
////                    int png = mHelper.getNewPng();
////                    Log.e(TAG, "=======png : " + png);
//
                    Bitmap bitmap = ((BitmapDrawable) getDrawable(R.drawable.ll)).getBitmap();
                    int pixel = bitmap.getPixel(53, 53);
                    Log.e(TAG, "=======pixel : " + pixel);
                    int aa = (pixel >> 24) & 0xFF;
                    int rr = (pixel >> 16) & 0xFF;
                    int gg = (pixel >> 8) & 0xFF;
                    int bb = (pixel >> 0) & 0xFF;
                    Log.e(TAG, "=======pixel : " + aa + "," + rr + "," + gg + "," + bb);
                    int w = bitmap.getWidth(), h = bitmap.getHeight();
                    int[] pix = new int[w * h];
                    Log.e(TAG, "=======png : " + pix.length);
                    bitmap.getPixels(pix, 0, w, 0, 0, w, h);
                    int[] ints = mNativeHelper.testBitmap(pix, w, h);
                    Log.e(TAG, "=======ints : " + ints.length);
                    Log.e(TAG, "=======ints pixels --- : " + ints[1024 * 53 + 53]);
//
//
                    Bitmap bitmap2 = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
                    bitmap2.setPixels(ints, 0, w, 0, 0, w, h);
                    try {
                        FileOutputStream out = new FileOutputStream("/data/user/0/com.asysbang.touch/files/123.png");
                        bitmap2.compress(Bitmap.CompressFormat.PNG, 100, out);
                        Log.e(TAG, "=======pixel :/data/user/0/com.asysbang.touch/files/123.png ");
                    } catch (FileNotFoundException e) {
                        throw new RuntimeException(e);
                    }
//                    byte[] data = new byte[ints.length];
//                    for (int i =0;i<ints.length;i++) {
//                        data[i] = (byte) ints[i];
//                    }
//                    int r = ints[1024*4*53 +53*4];
//                    int g = ints[1024*4*53 +53*4+1];
//                    int b = ints[1024*4*53 +53*4+2];
//                    int a = ints[1024*4*53 +53*4+3];
//                    int rgb = ((r << 16) | (g << 8) | (b )|(0xFF<<24));
//                    Log.e(TAG, "=======rgb : " +rgb);
//                    Log.e(TAG, "=======data : " +r+" , "+g+" , "+b+", "+a);
                } else {
                    Log.e(TAG, "=======connectServer failed : " + mIsConnected);
                }
                Log.e(TAG, "=======over: ");
            }
        }).start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.e(TAG, "=======onStartCommand:" + getFilesDir().getAbsolutePath());
        SuHelper.getInstance().runSu();
        //SuHelper.getInstance().getScreencap();
//        SuHelper.getInstance().runCmd("sh " + getFilesDir().getAbsolutePath() + "/run.sh\n");
        SuHelper.getInstance().runCmd("chmod 777 /data/user/0/com.asysbang.touch/files/karl\n");
        SuHelper.getInstance().runCmd("/data/user/0/com.asysbang.touch/files/karl\n");
        //runTestClient();
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onMoved(int x, int y) {
        mLp.x = mLp.x + x;
        mLp.y = mLp.y + y;
        mWindowManager.updateViewLayout(mFloatView, mLp);
    }

    private void connectNativeServer() {
        if(mIsConnected != 1) {
            mIsConnected = mNativeHelper.connectServer();
        }

    }
    @Override
    public void onStarted() {
        connectNativeServer();
        if (mIsConnected == 1) {
            mScreenWidth = mNativeHelper.getWidth();
            mScreenHeight = mNativeHelper.getHeight();
            Log.e(TAG, "=======native get width is  " + mScreenWidth + " , height is " + mScreenHeight);
        } else {
            Log.e("", "===========mIsConnected is false");
        }
    }

    @Override
    public void onStopped() {

    }

    @Override
    public void onConfig() {
        connectNativeServer();
        if (mIsConnected == 1) {
            Log.e("", "===========onConfig");
            char[] rgb = mNativeHelper.getRgb(53, 53);
            Log.e(TAG, "=======native rgb at [53,53] is : " + (int) rgb[0] + " , " + (int) rgb[1] + " , " + (int) rgb[2]);
        } else {
            Log.e("", "===========mIsConnected is false");
        }


    }

    @Override
    public void onTest() {
        Log.e("", "===========onTest");
        long time1 = System.currentTimeMillis();
        connectNativeServer();
        if (mIsConnected != 1) {
            Log.e("", "===========mIsConnected is false");
            return;
        }
        if (mScreenWidth <= 0 || mScreenHeight <= 0) {
            Log.e("", "===========mScreenWidth or  mScreenHeight is unknown");
            return;
        }
        int[] ints = mNativeHelper.getBitmapPixels(mScreenWidth, mScreenHeight);
        Log.e("", "===========onTest get pixels is   "+(System.currentTimeMillis()-time1));
        Bitmap bitmap2 = Bitmap.createBitmap(mScreenWidth, mScreenHeight, Bitmap.Config.ARGB_8888);
        bitmap2.setPixels(ints, 0, mScreenWidth, 0, 0, mScreenWidth, mScreenHeight);
        try {
            long time = System.currentTimeMillis();
            FileOutputStream out = new FileOutputStream("/data/user/0/com.asysbang.touch/files/"+time+".png");
            bitmap2.compress(Bitmap.CompressFormat.PNG, 100, out);
            Log.e(TAG, "=======pixel :/data/user/0/com.asysbang.touch/files/"+time+".png ");
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void onGo() {
        Log.e("", "===========onGo==");
    }
}
