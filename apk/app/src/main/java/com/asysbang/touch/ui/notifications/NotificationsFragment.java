package com.asysbang.touch.ui.notifications;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.asysbang.touch.R;
import com.asysbang.touch.databinding.FragmentNotificationsBinding;

public class NotificationsFragment extends Fragment {

    private FragmentNotificationsBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        NotificationsViewModel notificationsViewModel =
                new ViewModelProvider(this).get(NotificationsViewModel.class);

        binding = FragmentNotificationsBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final TextView textView = binding.textNotifications;
        notificationsViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);


        //testOpencv();
        return root;
    }

    private void testOpencv() {
        Drawable drawable = getContext().getDrawable(R.drawable.abc1);//-185 +100 40 40
        Bitmap bitmap = ((BitmapDrawable) drawable).getBitmap();
//        Mat siftMat = new Mat();
//        Utils.bitmapToMat(bitmap, siftMat);
//        Log.e("","======"+siftMat.width());
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}