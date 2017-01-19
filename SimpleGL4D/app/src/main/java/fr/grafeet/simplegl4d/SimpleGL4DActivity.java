package fr.grafeet.simplegl4d;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;

public class SimpleGL4DActivity extends AppCompatActivity {
    private SimpleGL4DView _glView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        _glView = new SimpleGL4DView(this);
        setContentView(_glView);
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public void onRestart() { super.onRestart(); }
}
