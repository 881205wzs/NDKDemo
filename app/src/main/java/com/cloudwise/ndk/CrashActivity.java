package com.cloudwise.ndk;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.InputStream;

public class CrashActivity extends AppCompatActivity implements View.OnClickListener {

    TextView txt;
    Button btn, btn_debug, btn_close, btn_ver_code, btn_ver, btn_logcat;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_crash);

        // Example of a call to a native method
        txt = (TextView) findViewById(R.id.sample_text);
        btn = (Button)findViewById(R.id.btn);
        btn.setOnClickListener(this);

        btn_debug = (Button)findViewById(R.id.btn_debug);
        btn_debug.setOnClickListener(this);

        btn_close = (Button)findViewById(R.id.btn_close);
        btn_close.setOnClickListener(this);

        btn_ver = (Button)findViewById(R.id.btn_ver);
        btn_ver.setOnClickListener(this);

        btn_ver_code = (Button)findViewById(R.id.btn_ver_code);
        btn_ver_code.setOnClickListener(this);

        btn_logcat = (Button)findViewById(R.id.btn_string);
        btn_logcat.setOnClickListener(this);

        btn = (Button)findViewById(R.id.btn_void);
        btn.setOnClickListener(this);
    }

    private void btnClick(){
        txt.setText(NDKUtil.stringFromJNI());
        //Log.e("CLOUDWISE", "stringFromJNI-------");
    }

    private void btnDebug(){
        NDKUtil.setJNILogEnable(1);
    }

    private void btnClose(){
        NDKUtil.setJNILogEnable(0);
    }

    private void btnVer(){
        txt.setText(NDKUtil.getVersion(1));
    }

    private void btnVerCode(){
        txt.setText(NDKUtil.getVersionCode()+"");
    }

    private void btnString(){
        NDKUtil.callJavaString();
    }

    private void btnVoid(){
        NDKUtil.callJavaVoid();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btn:
                btnClick();
                break;
            case R.id.btn_debug:
                btnDebug();
                break;
            case R.id.btn_close:
                btnClose();
                break;
            case R.id.btn_ver:
                btnVer();
                break;
            case R.id.btn_ver_code:
                btnVerCode();
                break;
            case R.id.btn_string:
                btnString();
                break;
            case R.id.btn_void:
                btnVoid();
                break;
        }
    }
}
