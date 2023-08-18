package com.asysbang.touch;

import android.app.Service;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.IBinder;
import android.util.Log;

import com.asysbang.touch.su.SuHelper;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;

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

                    int width = mHelper.getWidth();
                    int height = mHelper.getHeight();
                    Log.e(TAG, "=======runTestClient width is " + width +" , height is "+height);
//                    char[] rgb = mHelper.getRgb(53, 53);//????????????? 应该是byte 还是char
//                    Log.e(TAG, "=======runTestClient rgb : " + (int) rgb[0] + " , " + (int) rgb[1] + " , " + (int) rgb[2]);
////                    Log.e(TAG, "=======png start: ");
////                    int png = mHelper.getNewPng();
////                    Log.e(TAG, "=======png : " + png);
//
//                    Bitmap bitmap = ((BitmapDrawable) getDrawable(R.drawable.ll)).getBitmap();
//                    int pixel = bitmap.getPixel(53, 53);
//                    Log.e(TAG, "=======pixel : " +pixel);
//                    int aa = (pixel>>24)&0xFF;
//                    int rr = (pixel>>16)&0xFF;
//                    int gg = (pixel>>8)&0xFF;
//                    int bb = (pixel>>0)&0xFF;
//                    Log.e(TAG, "=======pixel : " +aa+","+rr+","+gg+","+bb);
//                    int w = bitmap.getWidth(), h = bitmap.getHeight();
//                    int[] pix = new int[w * h];
//                    Log.e(TAG, "=======png : " +pix.length);
//                    bitmap.getPixels(pix, 0, w, 0, 0, w, h);
//                    int[] ints = mHelper.testBitmap(pix, w, h);
//                    Log.e(TAG, "=======ints : " +ints.length);
//                    Log.e(TAG, "=======ints pixels --- : " +ints[1024*53 +53]);
//
//
//                    Bitmap bitmap2=Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
//                    bitmap2.setPixels(ints,0,w,0,0,w,h);
//                    try {
//                        FileOutputStream out = new FileOutputStream("/data/user/0/com.asysbang.touch/files/123.png");
//                        bitmap2.compress(Bitmap.CompressFormat.PNG, 100, out);
//                    } catch (FileNotFoundException e) {
//                        throw new RuntimeException(e);
//                    }
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
                    Log.e(TAG, "=======connectServer failed : " + connected);
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
        SuHelper.getInstance().runCmd("sh " + getFilesDir().getAbsolutePath() + "/run.sh\n");
        runTestClient();
        return super.onStartCommand(intent, flags, startId);
    }
}
