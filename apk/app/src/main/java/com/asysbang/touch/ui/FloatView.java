package com.asysbang.touch.ui;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;

import com.asysbang.touch.R;

public class FloatView extends FrameLayout implements View.OnClickListener, View.OnTouchListener {

    private FloatViewListener mListener;
    private int downX, downY;

    private ImageView mBtnStart, mBtnSettings, mBtnTest, mBtnGo;

    public FloatView(Context context, FloatViewListener listener) {
        super(context);
        mListener = listener;
        setOnTouchListener(this);
        LayoutInflater.from(context).inflate(R.layout.float_main, this);
        mBtnStart = findViewById(R.id.float_start);
        mBtnStart.setOnClickListener(this);
        mBtnSettings = findViewById(R.id.float_settings);
        mBtnSettings.setOnClickListener(this);
        mBtnTest = findViewById(R.id.float_test);
        mBtnTest.setOnClickListener(this);
        mBtnGo = findViewById(R.id.float_go);
        mBtnGo.setOnClickListener(this);
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
        int id = v.getId();
        if (id == R.id.float_start) {
            mListener.onStarted();
        } else if (id == R.id.float_settings) {
            mListener.onConfig();
        } else if (id == R.id.float_test) {
            mListener.onTest();
        } else if (id == R.id.float_go) {
            mListener.onGo();
        }
    }

    public interface FloatViewListener {
        void onMoved(int x, int y);

        void onStarted();

        void onStopped();

        void onConfig();

        void onTest();

        void onGo();
    }
}
