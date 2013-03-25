
package cn.yo2.aquarium.phoneloc;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

    private EditText mEtPhoneNumber;
    private TextView mTvPhoneLoc;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        mEtPhoneNumber = (EditText) findViewById(R.id.et_number_input);
        mTvPhoneLoc = (TextView) findViewById(R.id.tv_result);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    public void onBtnQueryClicked(View v) {
        String number = mEtPhoneNumber.getText().toString();
        String location = PhoneLocation.getLocationFromPhone(this, number);
        mTvPhoneLoc.setText(location);
    }

}
