package com.anadolu.cow;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import com.memetix.mst.language.Language;
import com.memetix.mst.translate.Translate;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class SentToTranslatorActivity extends Activity {
	private static final String TAG = SentToTranslatorActivity.class.getSimpleName();
	String translatedText;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		/*
		Translate.setClientId("4600b238-ee42-4ab5-bfd2-e33e0cafba32");
        Translate.setClientSecret("HnI6hNj+xSq2yiG7jxArnhHq59Knu+uxQFp0LswCw/A=");
        
		File sdcardPath = Environment.getExternalStorageDirectory();
		File file = new File(sdcardPath, "original.txt");*/
		/*
		FileWriter fwriter;
		try {
			fwriter = new FileWriter(file);
			BufferedWriter out = new BufferedWriter(fwriter);
	        out.write("Hello world");
	        out.close();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
       */
       /* file = new File(sdcardPath, "original.txt");
		StringBuilder text = new StringBuilder();
		TextView tv = (TextView)findViewById(R.id.showText);
		try {
		    BufferedReader br = new BufferedReader(new FileReader(file));
		    String line;

		    while ((line = br.readLine()) != null) {
		        text.append(line);
		        text.append('\n');
		    }
		}
		catch (IOException e) {
			//
		}	
		Button btnTrans = (Button)findViewById(R.id.buttonTranslate);
		btnTrans.setClickable(true);
		btnTrans.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
	            Log.i("LOG", translatedText);
	            try {
	             translatedText = Translate.execute("I should probably set this to something a little less profane", Language.ENGLISH, Language.FRENCH);
	            } catch(Exception e) {
	             translatedText = e.toString();
	            }
			}
		});
		
		tv.setText(translatedText);
		*/
		final TextView tv = (TextView)findViewById(R.id.showText);
        new MyAsyncTask() {
            protected void onPostExecute(Boolean result) {
             tv.setText(translatedText);
            }
        }.execute();
	}
	
	 class MyAsyncTask extends AsyncTask<Void, Integer, Boolean> {
	        @Override
	        protected Boolean doInBackground(Void... arg0) {
	        	Translate.setClientId("4600b238-ee42-4ab5-bfd2-e33e0cafba32");
	            Translate.setClientSecret("HnI6hNj+xSq2yiG7jxArnhHq59Knu+uxQFp0LswCw/A=");
	            try {
	             translatedText = Translate.execute("I love cow", Language.ENGLISH, Language.TURKISH);
	            } catch(Exception e) {
	             translatedText = e.toString();
	            }
	            return true;
	        }	
	    }


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

}