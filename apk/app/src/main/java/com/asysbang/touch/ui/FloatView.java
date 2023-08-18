package com.asysbang.touch.ui;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

import com.asysbang.touch.R;

public class FloatView extends FrameLayout implements View.OnClickListener, View.OnTouchListener {

    private FloatViewListener mListener;
    private int downX, downY;

    public FloatView(Context context, FloatViewListener listener) {
        super(context);
        mListener = listener;
        setOnTouchListener(this);
        LayoutInflater.from(context).inflate(R.layout.float_main, this);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int action = event.getAction();
        int x = (int) event.getRawX();
        int y = (int) event.getRawY();
        switch (action) {
            case MotionEvent.ACTION_DOWN:
                downX = x;
                downY = y;
                break;
            case MotionEvent.ACTION_MOVE:
                int deltaX = (int) (event.getRawX() - downX);
                int deltaY = (int) (event.getRawY() - downY);
                mListener.onMoved(deltaX, deltaY);
                downX = x;
                downY = y;
                break;
            case MotionEvent.ACTION_UP:
                break;
            default:
                break;
        }
        return false;
    }

    @Override
    public void onClick(View v) {

    }

    public interface FloatViewListener {
        void onMoved(int x, int y);

        void onStarted();

        void onStopped();

        void onConfig();
    }
}
