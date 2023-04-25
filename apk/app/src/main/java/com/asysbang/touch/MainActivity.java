package com.asysbang.touch;

import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

import com.asysbang.touch.su.SuHelper;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.asysbang.touch.databinding.ActivityMainBinding;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        BottomNavigationView navView = findViewById(R.id.nav_view);
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        AppBarConfiguration appBarConfiguration = new AppBarConfiguration.Builder(
                R.id.navigation_home, R.id.navigation_dashboard, R.id.navigation_notifications)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_activity_main);
        NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration);
        NavigationUI.setupWithNavController(binding.navView, navController);
        copyAssets();
        startMainService();
    }

    private void copyAssets() {
        try {
            File dir = getFilesDir();
            Log.e("","======getFilesDir="+dir.getAbsolutePath());
            File karlFile=  new File(dir.getAbsolutePath()+"/karl");
            if (karlFile.exists()) {
                Log.e("","======exists=");
                return;
            }
            AssetManager assetManager = getAssets();
            InputStream inputStream = assetManager.open("karl");
            FileOutputStream fos = new FileOutputStream(dir.getAbsolutePath()+"/karl");
            byte[] buf = new byte[2048];
            int len = -1;
            while ((len = inputStream.read(buf))!=-1) {
                fos.write(buf,0,len);
            }
            fos.flush();
            fos.close();
            inputStream.close();

            inputStream = assetManager.open("run.sh");
            fos = new FileOutputStream(dir.getAbsolutePath()+"/run.sh");
            while ((len = inputStream.read(buf))!=-1) {
                fos.write(buf,0,len);
            }
            fos.flush();
            fos.close();
            inputStream.close();


        } catch (IOException e) {
            throw new RuntimeException(e);
        }

    }

    private void startMainService() {
        startService(new Intent(this,MainService.class));
    }

}