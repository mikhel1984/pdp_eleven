package img;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.awt.Color;
import java.awt.image.ColorConvertOp;
import java.awt.color.ColorSpace;
import java.io.File;
import java.io.FileOutputStream;

import java.io.FileWriter;
import java.io.IOException;

/**
 * Created by stas on 11/15/16.
 */
public class ImgConvertor {
    private BufferedImage bufimg;
    private int converted[];
    private int bytes;

    ImgConvertor(String fname) {
        try{
            bufimg = ImageIO.read(new File(fname));
            bytes = bufimg.getHeight()*bufimg.getWidth() / 8;

            converted = new int[bytes];
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        toBinary();
        printToFile("maze.h");
        //printToBin("maze");

    }

    private void toBinary() {

        int arrpos = 0, bpos = 7;
        int sum = 0;
        for(int i = 0; i < bufimg.getWidth(); ++i) {
            for(int j = 0; j < bufimg.getHeight(); ++j) {
                sum += (isWhite(i,j) << bpos);
                if(--bpos < 0) {
                    converted[arrpos] = sum;
                    arrpos++;
                    bpos = 7;
                    sum = 0;
                }
            }
        }

    }

    private int isWhite(int w, int h) {
        Color clr = new Color(bufimg.getRGB(w,h));
        return (clr.getRed() > 128 && clr.getBlue() > 128 && clr.getGreen() > 128) ? 1 : 0;
    }

    private void printToBin(String fname) {
        byte seq[] = new byte[bytes];

        int k = 0;

        for(int i = 0; i < bytes; ++i) {
            seq[i] = (byte) converted[i];
            k++;
        }
        System.out.println(k);

        try {
            //FileWriter writer = new FileWriter(fname, false);
            //writer.write(seq);
            //writer.flush();
            FileOutputStream fos = new FileOutputStream(fname);
            fos.write(seq, 0, seq.length);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void printToFile(String fname) {
        try {
            int k = 1;
            FileWriter writer = new FileWriter(fname, false);
            writer.write("char maze[] = {\n");
            for(int i : converted) {
                writer.append(String.format("0x%x,", i));

                if((k % 16) == 0) {
                    writer.append("\n");
                }
                k++;
            }
            writer.append("};");
            writer.flush();

        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
