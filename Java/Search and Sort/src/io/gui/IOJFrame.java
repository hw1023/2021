package io.gui;

import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.util.ArrayList;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.DefaultTableModel;

import stu.javabean.Student;
import stu.util.DataSource;
import stu.util.StudentUtil;

public class IOJFrame extends JFrame {

	private JPanel contentPane;
	private JTextField textField;
	private JTable table;
	BufferedReader br;
	BufferedWriter sw;
	StudentUtil su;
	ArrayList<Student> all;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					IOJFrame frame = new IOJFrame();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public IOJFrame() {
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 552, 416);
		
		JMenuBar menuBar = new JMenuBar();
		setJMenuBar(menuBar);
		
		JMenu mnFile = new JMenu("File");
		menuBar.add(mnFile);
		
		JMenuItem mntmOpen = new JMenuItem("Open");
		mntmOpen.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				 JFileChooser chooser = new JFileChooser();
			     FileNameExtensionFilter filter = new FileNameExtensionFilter("TXT Files", "txt");
			     chooser.setFileFilter(filter);
			     int returnVal = chooser.showOpenDialog(null);
			     if(returnVal == JFileChooser.APPROVE_OPTION) {
			    	 String path=chooser.getSelectedFile().getAbsolutePath();
			    	 br=DataSource.getDataReader(path, "UTF-8");
			    	 su=new StudentUtil(br);
			    	 su.closeBr(br);
			    	 JOptionPane.showMessageDialog(null, "File opened", "WOw", JOptionPane.INFORMATION_MESSAGE);
			     }//end of if

			}
		});
		mnFile.add(mntmOpen);
		
		JMenuItem mntmSave = new JMenuItem("Save");
		mntmSave.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				JFileChooser chooser = new JFileChooser();
			     FileNameExtensionFilter filter = new FileNameExtensionFilter("TXT Files", "txt");
			     chooser.setFileFilter(filter);
			     int returnVal = chooser.showSaveDialog(null);
			     if(returnVal == JFileChooser.APPROVE_OPTION) {
			    	 String path=chooser.getSelectedFile().getAbsolutePath();
			    	 sw=DataSource.setDataWriter(path, "UTF-8");
			    	 su.writerData(all, sw);
			    	 su.closeBw(sw);
			    	 JOptionPane.showMessageDialog(null, "File saved", "WOw", JOptionPane.INFORMATION_MESSAGE);
			     }//end of if

			}
		});
		mnFile.add(mntmSave);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);
		
		JLabel label = new JLabel("\u8BF7\u8F93\u5165\u5B66\u53F7\uFF1A");
		label.setBounds(30, 8, 121, 18);
		contentPane.add(label);
		
		textField = new JTextField();
		textField.setBounds(120, 7, 187, 23);
		contentPane.add(textField);
		textField.setColumns(10);
		
		JButton button = new JButton("\u6A21\u7CCA\u67E5\u8BE2");
		button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				String id=textField.getText().trim();
				
				if(id.equals("")) {
					all=su.queryAll();
				}else {
					all=su.querById(id);
					
				}
				all=su.orderByScore();//按成绩排序
				Vector<String> col=new Vector<String>();
				col.add("Id");
				col.add("Name");
				col.add("Major");
				col.add("Mark");
				DefaultTableModel dm=new DefaultTableModel(col,0);
				for(Student s:all) {
					String[] line= {s.getId(),s.getName(),s.getMajor(),s.getMark()};
					dm.addRow(line);
				}
				table.setModel(dm);
			}//end of action
		});//end of listener
		button.setBounds(317, 7, 97, 23);
		contentPane.add(button);
		
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(10, 38, 518, 308);
		contentPane.add(scrollPane);
		
		table = new JTable();
		scrollPane.setViewportView(table);
		
		JButton button_1 = new JButton("\u7CBE\u786E\u67E5\u8BE2");
		button_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				String id=textField.getText().trim();
				Vector<String> col=new Vector<String>();
				col.add("Id");
				col.add("Name");
				col.add("Major");
				col.add("Mark");
				DefaultTableModel dm=new DefaultTableModel(col,0);
				if(id.equals("")) {
					all=su.clearSel(id);//输入为空时，精确查询结果为空					
					for(Student s:all) {
						String[] line= {s.getId(),s.getName(),s.getMajor(),s.getMark()};
						dm.addRow(line);
					}
					table.setModel(dm);
				}else {
					all=su.equalToId(id);//精确查询
					all=su.orderByScore();//按成绩排序					
					for(Student s:all) {
						String[] line= {s.getId(),s.getName(),s.getMajor(),s.getMark()};
						dm.addRow(line);
					}
					table.setModel(dm);
				}//end if
				
			}//end of action
		});//end of listener
		button_1.setBounds(424, 7, 97, 23);
		contentPane.add(button_1);
	}
}
