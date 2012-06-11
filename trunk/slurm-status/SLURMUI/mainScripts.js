//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%					        						  	  %%%
//%%  				  FUNCTIONS USED IN SLURMUI/home.html     				  %%%
//%%  					       							          %%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



//===========================================================================================================
//
// 	Updates Slurm Status bar
// 	Calls SLURMUI_slurm_ping
//
//===========================================================================================================


function updatePing(){
        var slurmping = "";
   	var message = "";

        $.getJSON('../../cgi-bin/slurm/SLURMUI_slurm_ping',function(data){
  		$.each(data, function(key, val) {
		if(key == "slurm_ping(1)")
		{
			if (val == 0) message += "Primary controller is online. ";
			else message += "Primary controller is offline. ";
		}
		if(key == "slurm_ping(2)")
		{
			if (val == 0) message += "Secondary controller is online.";
			else message += "Secondary controller is offline.";
		}                
                });
		var d = new Date();
                document.getElementById("slurmping").innerHTML="<h3 style='color: #79BCFF;'>"+message+"</h3><h5>Message Time: "+ d+"</h5>"; 

	});

}

//===========================================================================================================
//
// 	Fills Slurm Controller Status Page
// 	Calls SLURMUI_slurmd_status
//
//===========================================================================================================


function updateControllerStatus(){

	var node = "";
	
	$.getJSON('../../cgi-bin/slurm/SLURMUI_slurmd_status',function(data){
	var start = "<table  border='3' style='border-color: #79bbff; margin-left:50px;' class='ui-widget' ' id='static'><tbody class='ui-widget-content'>";
  		$.each(data, function(key, val) {
                node = node + "<tr><td class='ui-widget-header' width='200px' align='center'>"+ key+" </td><td width='300px' align='center'>"+val+" </td></tr>";
                document.getElementById("tabs-1data").innerHTML=start + node+ "</tbody></table></br>";

 		});
 	});
   

}	



//===========================================================================================================
//
// 	String to ISO Date
//
//===========================================================================================================

function stringToDate(string){
	var matches;
	if(matches = string.match(/^(\d{4,4})-(\d{2,2})-(\d{2,2})$/)) {

		return new Date(matches[1],matches[2]-1,matches[3]);

	}	
	else 
	{	
		return null;
	};


}



//===========================================================================================================
//
//	Fills Node Page	
//	Calls SLURMUI_get_node_info
//
//===========================================================================================================


function updateNodeStatus(){

	var nodecount = 0;	
	var partitioncount=0;

	var tabcreated = 0;
	var i = 0;
	var k = 0;
	var allNodes = 0;
	var rowcount =0 ;

	document.getElementById("tabs-2data").innerHTML = "";
        var partitiontabs = "";
   	var partitiontabs2 = "";
	var start = "<div id='partitiontabs' class=\"vtab\">";
	var end = "</div>";

	var nodeStart = "<br/><h3 style='margin-left:50px;'>1 - 21</h3><table style='margin-left:50px;' border=\"1\"><tr>";

	var node = "";
	document.getElementById("tabs-2data").innerHTML = start+end;

        $.getJSON('../../cgi-bin/slurm/SLURMUI_get_node_info',function(data){
	
  		$.each(data, function(key, val) {  
	
		if (tabcreated == 0)
		{ 
		var partitionNames = key.split('~');
			for(i = 0; i< val; i ++)
			{
			partitiontabs += "<li id='nodebutton-"+i+"'><br /><br /><span>"+partitionNames[i]+"</span></li>";
			partitiontabs2 +="<div id='partitiontabs-"+i+"' ></br></br><div style='margin-left:50px;'>Currently, there are no nodes in this partition.</div> </div>";
		
			}
			partitiontabs = "<ul>" + partitiontabs +  "</ul>" + partitiontabs2;

			document.getElementById("tabs-2data").innerHTML = start+partitiontabs+end;
		tabcreated = 1;

		}
		else{
	
	
		$.each(val, function(nodekey, nodeval) {

			var title="";
			$.each(nodeval, function(nodedetailkey, nodedetailval) { 
			if(nodedetailval == "")
			title += nodedetailkey + ": -      ";
		        else if(nodedetailkey == "Jobs"){
			 var trimval = nodedetailval.slice(0,-1);
			title += nodedetailkey + ": "+trimval+ "      ";
			}		
			else 
			title += nodedetailkey + ": "+nodedetailval+ "      ";
			});
			
			if (nodekey.indexOf("allocated") != -1)
			node += "<td class='tooltip' title=\""+title+"\" id=node"+allNodes+" bgcolor='#DF0101'><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+nodekey.split('~')[1]+"</span></td>";
			else if (nodekey.indexOf("idle") != -1)
			node += "<td class='tooltip' title=\""+title+"\" id=node"+allNodes+" bgcolor='#80FF00'><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+nodekey.split('~')[1]+"</span></td>";
			else if (nodekey.indexOf("down") != -1)
			node += "<td class='tooltip' title=\""+title+"\" id=node"+allNodes+" bgcolor='#F5BCA9'><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+nodekey.split('~')[1]+"</span></td>";
			else if (nodekey.indexOf("unknown") != -1)
			node += "<td class='tooltip'title=\""+title+"\" id=node"+allNodes+" bgcolor='#58FAF4'><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+nodekey.split('~')[1]+"</span></td>";
			else { node += "<td   class='tooltip' title=\""+title+"\" id=node"+allNodes+" bgcolor=#AAAAAA><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+nodekey.split('~')[1]+"</span></td>";}
			nodecount ++;
			
			if(nodecount%7==0)
			{
			rowcount++;
			if(rowcount%3 == 0)
			{
				node = node + "</tr></table></br></br><h3 style='margin-left:50px;'>"+(rowcount*7)+" - "+((rowcount+3)*7)+"</h3><table style='margin-left:50px;' border=\"1\"><tr>";
			}
			else node = node + "</tr><tr>";
		
			}	
                	document.getElementById("partitiontabs-"+partitioncount).innerHTML=nodeStart+ node + "</tr></table>";

			allNodes ++;
			
		});
	
			partitioncount++;
			node = "";
			partitiontabs="";
			nodecount = 0;
			rowcount = 0;
	
	 	}


	var $items = $('#partitiontabs>ul>li');
            $items.click(function() {
                $items.removeClass('selected');
                $(this).addClass('selected');
		lastTab = $(this).attr('id').split('-')[1];

                var index = $items.index($(this));
                $('#partitiontabs>div').hide().eq(index).show();
            });				
	$("#nodebutton-" + lastTab).click();		

 	});
	

	if(allNodes==0)
	{	
	
		document.getElementById("tabs-2data").innerHTML=start +  "</br></br><div style='margin-left:50px;'>Currently, there are no nodes.</div></div>";
		return false;
	
	}


 });
	
	return true;	
  	
}

//===========================================================================================================
//
// 	Fills Job Info Page 
// 	Calls SLURMUI_get_job_info
//
//===========================================================================================================


var lastTab = 0;

function updateJobStatus(){

	var jobcount = 0;	
	var jobpartitioncount=0;
	var jobpartitiontabs = "";
	var jobpartitiontabs2 = "";
	var job = "";
	var tabcreated = 0;
	var i = 0;
	var k = 0;
	var allJobs = 0;
	var rowcount =0 ;

	document.getElementById("tabs-3data").innerHTML = "";

	var start = "<div id='jobpartitiontabs' class=\"vtab\">";
	var end = "</div>";
	var jobStart = "<br/><h3 style='margin-left:50px;'>1 - 24</h3><table border=\"1\" style='margin-left:50px;'><tr>";
	document.getElementById("tabs-3data").innerHTML = start+end;
        $.getJSON('../../cgi-bin/slurm/SLURMUI_get_job_info',function(data){
		
  		$.each(data, function(key, val) {  
	
		if (tabcreated == 0)
		{ 
		var jobpartitionNames = key.split('~');
			for(i = 0; i< val; i ++)
			{
			jobpartitiontabs += "<li id='jobbutton-"+i+"'><br /><br /><span>"+ jobpartitionNames[i] +"</span></li>";
			jobpartitiontabs2 +="<div id='jobpartitiontabs-"+i+"' ></br></br><div style='margin-left:50px;'>Currently, there are no jobs in this partition.</div></div>"
			
			}
			jobpartitiontabs = "<ul>" + jobpartitiontabs +  "</ul>" + jobpartitiontabs2;

			document.getElementById("tabs-3data").innerHTML = start+jobpartitiontabs+end;
		tabcreated = 1;

		}
		else{
	
	
		$.each(val, function(jobkey, jobval) {

			var title="";
			$.each(jobval, function(jobdetailkey, jobdetailval) { 
			if(jobdetailval == "")
			title += jobdetailkey + ": -      ";
			else 
			title += jobdetailkey + ": "+jobdetailval+ "      ";
			});
			
			if (jobkey.indexOf("waiting") != -1)
			job += "<td class='tooltip' title=\""+title+"\" id=node"+allJobs+" bgcolor='#FF0040'><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+jobkey.split('~')[1]+"</span></td>";
			else if (jobkey.indexOf("running") != -1)
			job += "<td class='tooltip' title=\""+title+"\" id=node"+allJobs+" bgcolor=#80FF00><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+jobkey.split('~')[1]+"</span></td>";
			else if (jobkey.indexOf("stopped") != -1)
			job += "<td class='tooltip' title=\""+title+"\" id=node"+allJobs+" bgcolor=#F5BCA9><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+jobkey.split('~')[1]+"</span></td>";
			else { job += "<td   class='tooltip' title=\""+title+"\" id=node"+allJobs+" bgcolor=#AAAAAA><a href='#' ></a><span style='font-family: verdana; font-size:12px'>"+jobkey.split('~')[1]+"</span></td>";}
			jobcount ++;
			
			if(jobcount%8==0)
			{
			rowcount++;
			if(rowcount%3 == 0)
			{
				job = job + "</tr></table></br></br><h3 style='margin-left:50px;'>"+(rowcount*8)+" - "+((rowcount+3)*8)+"</h3><table border=\"1\" style='margin-left:100px;'><tr>";
			}
			else job = job + "</tr><tr>";
		
			}	
                	document.getElementById("jobpartitiontabs-"+jobpartitioncount).innerHTML=jobStart+ job + "</tr></table>";

			allJobs ++;
			
		});
			jobpartitioncount++;
			job = "";
			jobpartitiontabs="";
			jobcount = 0;
			rowcount = 0;
	 }


	var $items = $('#jobpartitiontabs>ul>li');
            $items.click(function() {
                $items.removeClass('selected');
                $(this).addClass('selected');
		lastTab = $(this).attr('id').split('-')[1];

                var index = $items.index($(this));
                $('#jobpartitiontabs>div').hide().eq(index).show();
            });			
	$("#jobbutton-" + lastTab).click();	
 	});
	

	if(allJobs==0)

{	
	document.getElementById("tabs-3data").innerHTML=start +  "</br></br><div style='margin-left:50px;'>Currently, there are no jobs.</div></div>";
	return false;
}


 });
	
	return true;	
  	
}




//===========================================================================================================
//
//	Fills Partition Page	
//	Calls SLURMUI_get_partition_info
//
//===========================================================================================================


function updatePartitionStatus(){

	var partitioncount=0;
	var tabcreated = 0;

	var i = 0;
	

	document.getElementById("tabs-5data").innerHTML = "";
        var partitiontabs = "";
   	var partitiontabs2 = "";
	var start = "<div id='partitionstatustabs' class=\"vtab\">";
	var end = "</div>";

	var table = "";
	document.getElementById("tabs-5data").innerHTML = start+end;

        $.getJSON('../../cgi-bin/slurm/SLURMUI_get_partition_info',function(data){
	
  		$.each(data, function(key, val) {  
			if (tabcreated == 0)
			{ 
				var partitionNames = key.split('~');
				for(i = 0; i< val; i ++)
				{	
					partitiontabs += "<li id='partitionbutton-"+i+"'><br /><br /><span>"+partitionNames[i]+"</span></li>";
					partitiontabs2 +="<div id='partitionstatustabs-"+i+"' > </div>";
				}
				partitiontabs = "<ul>" + partitiontabs +  "</ul>" + partitiontabs2;

				document.getElementById("tabs-5data").innerHTML = start+partitiontabs+end;
				tabcreated = 1;

			}	
			else{
				table = "</br></br><table  border='3' style='border-color: #79bbff; margin-left:50px;' class='ui-widget'><tbody class='ui-widget-content'>";
				$.each(val, function(partdetailkey, partdetailval) { 
					table += "<tr><td class='ui-widget-header' width = '200px' align='center'>"+partdetailkey+"</td><td width='300px' align='center'>"+partdetailval+"</td></tr>";
		
				});
                		document.getElementById("partitionstatustabs-"+partitioncount).innerHTML = table + "</tbody></table></br>";
				partitioncount++;
				table = "";
			}

	 	});

	var $items = $('#partitionstatustabs>ul>li');
            $items.click(function() {
                $items.removeClass('selected');
                $(this).addClass('selected');
		lastTab = $(this).attr('id').split('-')[1];

                var index = $items.index($(this));
                $('#partitionstatustabs>div').hide().eq(index).show();
            });				
	$("#partitionbutton-" + lastTab).click();		
 	});
 
}





//===========================================================================================================
//
//	Calls SLURMUI_write_log_file
//
//===========================================================================================================


function writeLog(){

 $.getJSON('../../cgi-bin/slurm/log/SLURMUI_write_log_file',function(data){});
      
}



//===========================================================================================================
//
//	Fills Current Statistics Page
//	Calls SLURMUI_init & SLURMUI_current_statistics
//	
//===========================================================================================================




function updateCurrentStatistics(){
document.getElementById("tabs-4data").innerHTML="<div id='reporttabs' class=\"vtab\"><ul><li id='reportbutton-0'><br /><br /><span>Node Report</span></li><li id='reportbutton-1'><br /><br /><span>Current Statistics</span></li></ul><div id='nodereportdata' > <div style='margin-left:25px;'><p>Start Date: <input type='text' id='datepicker'>&nbsp;&nbsp;&nbsp;&nbsp;For:&nbsp;&nbsp;<select id='daynum'><option value='1'>1 day</option>  <option value='3'>3 days</option><option value='7'>1 week</option></select> &nbsp;&nbsp;  <button type='button'  style='margin-left: 5px; margin-right: 5px; margin-top: 10px; 	padding: 2px;        padding-top:4px;	text-align: center;	font-family: Gill Sans,Arial,sans-serif;	font-size: 16px;	background-image: url(\"../jquery/jquery-ui-1.8.20.custom/css/hot-sneaks/images/ui-bg_diagonals-small_40_db4865_40x40.png\");	background-repeat: repeat; -moz-border-radius:4px; -webkit-border-radius:4px; border-radius:4px; cursor: pointer;' onclick='javascript:getNodeGraph();'>Bring Report!</button></p></div></br></br></br><div id='nodereportdatain'></div> </div><div id='currentstatisticsdata' ></br><h3>General Node Status</h3><table border='1'><tr><td><div id='generalpie' style='width: 600px; height: 300px; align:center;  -webkit-transform: translateZ(1000);'></div></td></tr></table></div></div>";

//***********************
//*****GET INITIAL DATE
 
        $.getJSON('../../cgi-bin/slurm/SLURMUI_init',function(data){



  		$.each(data, function(key, val) {


  $( "#datepicker" ).datepicker({dateFormat: 'yy-mm-dd', minDate: val, defaultDate: -1});

}); 

});


      var $items = $('#reporttabs>ul>li');
            $items.click(function() {
                $items.removeClass('selected');
                $(this).addClass('selected');
		lastTab = $(this).attr('id').split('-')[1];

                var index = $items.index($(this));
                $('#reporttabs>div').hide().eq(index).show();
            });				
	$("#reportbutton-" + lastTab).click();	



//********************************************************************
//DRAW CURRENT STATISTICS*********************************************
//********************************************************************



var i = 0;
 $.getJSON('../../cgi-bin/slurm/SLURMUI_current_statistics',function(data){

var chartArray = new Array();
var arrayindex = 0;

var chartDataArray = new Array();
var dataindex =  0;
  	$.each(data, function(key, val) { 
                 if(key == "General")
		 {	var count = 0;
			var myData=new Array();
			$.each(val, function(keydetail, valdetail) { 
				myData[count] = keydetail;
				count++;
				myData[count] = valdetail;
				count++;
			});		
			 var gen_chartData = [{title:myData[0],value:myData[1]},{title:myData[2],value:myData[3]},{title:myData[4],value:myData[5]},{title:myData[6],value:myData[7]}];	

    			var generalchart = new AmCharts.AmPieChart();
			generalchart.valueField = "value";
			generalchart.titleField = "title";
			generalchart.dataProvider = gen_chartData;
			generalchart.write("generalpie");

		}
		else
		{
		 document.getElementById("currentstatisticsdata").innerHTML += "<h3>Partition Name: "+key + "</h3><table border='1'><tr><td>Node Status</td><td>Job Status</td></tr><tr><td><div id='nodepie-"+i+"' style='width: 380px; height: 300px; -webkit-transform: translateZ(1000);'></div></td><td><div id='jobpie-"+i+"' style='width: 380px; height: 300px; -webkit-transform: translateZ(1000);'></div></td></tr></table></br>";

			var new_myData = new Array();
			var newcount = 0;

			$.each(val, function(keydetail, valdetail) {
			        new_myData[newcount] = keydetail;
				newcount++;
				new_myData[newcount] = valdetail;
				newcount++;
                        });

			if(new_myData[1] == 0 && new_myData[3] == 0 && new_myData[5] == 0 && new_myData[7] == 0 )
			{
				document.getElementById("nodepie-"+i).innerHTML = "</br></br><div style='margin-left:25px;'>Currently, there are no nodes in this partition.</div>"; 
			}
			else
			{
				chartDataArray[dataindex] = [{title:new_myData[0],value:new_myData[1]},{title:new_myData[2],value:new_myData[3]},{title:new_myData[4],value:new_myData[5]},{title:new_myData[6],value:new_myData[7]}];	

    				chartArray[arrayindex] = new AmCharts.AmPieChart();
				chartArray[arrayindex].valueField = "value";
				chartArray[arrayindex].titleField = "title";
				chartArray[arrayindex].dataProvider = chartDataArray[dataindex];
				chartArray[arrayindex].write("nodepie-"+i);
			}
			dataindex++;
			arrayindex++;

			if(new_myData[9] == 0 && new_myData[11] == 0 && new_myData[13] == 0  )
			{
				document.getElementById("jobpie-"+i).innerHTML = "</br></br><div style='margin-left:25px;'>Currently, there are no jobs in this partition.</div>"; 
			}
			else
			{

				chartDataArray[dataindex] = [{title:new_myData[8],value:new_myData[9]},{title:new_myData[10],value:new_myData[11]},{title:new_myData[12],value:new_myData[13]}];	
			  	 chartArray[arrayindex] = new AmCharts.AmPieChart();
				chartArray[arrayindex].valueField = "value";
				chartArray[arrayindex].titleField = "title";
				chartArray[arrayindex].dataProvider = chartDataArray[dataindex];
				chartArray[arrayindex].write("jobpie-"+i);
			}
			i++;	
			dataindex++;
			arrayindex++;
		}

               }); 
            	

 });



}



//===========================================================================================================
//
//	Fills Graph Page
//	Calss SLURMUI_get_node_names & SLURMUI_read_log_file
//
//===========================================================================================================


// GET NODE GRAPH
function getNodeGraph(){

//DRAW THE TABLES
	updateControllerStatus();
	var tablecreated = 0;
	var tablelabeled = 0;
	var noOfPartitions = 0;
	var nodeLabelCheck = 0;
        var timeCount= $("#daynum").val();
        var timeCount = timeCount*24*3;

	var i = 0;
	var k = 0;        
	var x = 0;
	var y = 0;
	
        var generalstring = "";
        var labelstring = "";
	var xstring = "";

        var jobcount = 0;	
        var partitioncount=0;
        var nodecount=0;
	var xlabelcount = 0;
        var infirstrow = 0;
	var colorcode = 1;

	document.getElementById("nodereportdatain").innerHTML = "";

	$.getJSON('../../cgi-bin/slurm/SLURMUI_get_node_names',function(data){
		
  		$.each(data, function(key, val) {  
	    
		var partitionData = key.split('~');
		if (partitionData[0] == "partitioncount")
		{ 
	
			var partitionNames = val.split('~');

			for(i = 0; i< partitionData[1]; i ++)
			{

				generalstring += "<div><h3>Partition "+i+": "+partitionNames[i]+"</h3></br><table width='624'  id='nodereport-"+partitionNames[i]+"'    cellpadding='0' cellspacing='0'  > <tr><td><div style='margin-left:50px;'>Currently, there are no nodes.</div></td></tr></table></div></br>";
			  	noOfPartitions ++;
			
			}

			document.getElementById("nodereportdatain").innerHTML += generalstring;
	
		}
		else if (partitionData[0] == "partitionname")
		{ 
                
			infirstrow =0;
                	xlabelcount = 0;
			labelstring = "";
			nodeLabelCheck = 0;
			var nodeNames = val.split('~');
                	xstring = "<tr><td></td>";	
			
			for(i = 0; i< partitionData[2]; i ++)
			{       
			
				labelstring += "<tr>";
			  
				for(k = 0; k< timeCount+1; k++){
               			
					if(nodeLabelCheck == 0){
		              	    		
						labelstring += "<td style='width:20px; font-size:13px;' >"+nodeNames[i]+" </td>";
				   
			            		nodeLabelCheck++;
			           	}
			           	else
			           	{
						
						if(timeCount == 72 ){
  							
							labelstring += "<td class='tooltip' style='border-top:outset 1px; border-bottom:outset 1px;' title=\"\" style='width:8;' id='"+partitionData[1]+(k-1)+nodeNames[i]+"'>&nbsp;&nbsp;</td>"; 
							
							if(xlabelcount == 5 && infirstrow== 0){
								
								xstring += "<td style='font-size:10px;' colspan='5' align='right'>"+(k-1)+"</td>";
								xlabelcount = 0;
							}

							xlabelcount++;
						}
				        	else if(timeCount == 216){

							labelstring += "<td class='tooltip' style='border-top:outset 1px; border-bottom:outset 1px;' title=\"\" style='width:3;' id='"+partitionData[1]+(k-1)+nodeNames[i]+"'>	</td>";
							
							if(xlabelcount == 15  && infirstrow== 0){
								
								xstring += "<td style='font-size:8px;' colspan='10' align='right'>"+(k-1)+"</td>";
								xlabelcount = 0;
							
							}

							xlabelcount++;
						}
						else if(timeCount == 504 ){

			          		 	labelstring += "<td class='tooltip' style='border-top:outset 1px; border-bottom:outset 1px;' title=\"\" style='width:3;' id='"+partitionData[1]+(k-1)+nodeNames[i]+"'></td>";
					
							if(xlabelcount == 30  && infirstrow== 0){
				
								xstring += "<td style='font-size:10px;' colspan='30' align='right'>"+(k-1)+"</td>";
								xlabelcount = 0;
							
							}

							xlabelcount++;
						}
			        
				   	} 
				
				} 
			   
				labelstring += "</tr>";
			  	infirstrow =1;
			   	nodeLabelCheck = 0;
			
			}
	
                  	 xstring += "<td  colspan='35' ></td><td style='font-size:10px;' align='right'>x 20min.</td></tr>";
			document.getElementById("nodereport-"+partitionData[1]).innerHTML = labelstring+xstring;
	
			tablelabeled++;
		
		} 

	}); 


//COLOR THE GRAPH

	var startdate =  $("#datepicker").val();
	var day_int = 1;

	var daynumlist= document.getElementById("daynum");
	var day_num = daynumlist.options[daynumlist.selectedIndex].value;
	
	if(day_num == null)
		var day_int = 1;
	else{
		var day_int = parseInt(day_num);
	}

	var cnt = day_num*24*3;

	if(startdate != "")
	{

		var start = stringToDate(startdate);
	}
	else var start = new Date();

	var end_t = new Date();
	end_t.setFullYear(start.getFullYear());
	end_t.setMonth(start.getMonth());

	end_t.setDate(start.getDate() + day_int);

      
	var ColorArray = new Array("#FF33CC","#6600FF","#00FFFF","#CC0000","#66FF00","#00CC33","#330066","#006600","#333300","#FF3333","#000000","#0000CC","#660099","#CC00CC","#990033","#00CCFF","#FFFF00","#FF6600","#FF0033","#FF0000","#00CC66","#009999","#33CCFF","#660033","#3399FF","#000066","#0033CC","#009933","#330000","#99FF33","#FF9933","#FF3333","#9900CC","#99CCFF","#00FFCC","#993300","#66FF99","#CC0000","#CC00FF","#009966","#FFFF66","#FFCC33","#0000CC","#FF0099","#CCFF33","#FF3300","#990066","#660000","#006666","#336600","#0066FF","#999900","#FF66FF","#CC0033");

        var random = new Alea();
	$.getJSON('../../cgi-bin/slurm/SLURMUI_read_log_file', { start: start, end: end_t, count: cnt },function(data){

  		$.each(data, function(key, val) { 

                        
			$.each(val, function(keydetail, valdetail) {  
		       
				if(valdetail != ""){	
				
				    $.each(valdetail, function(nodename, jobs) {  
                 
			        
					if(jobs == "_"){ 
		                    	//#99CCCC
						document.getElementById(keydetail+x+nodename).setAttribute("bgcolor","white");
				             
					}
					else
					{
		
	                                	var oldjobs= "";
					    	if(x != 0){
					    		oldjobs = document.getElementById(keydetail+(x-1)+nodename).getAttribute("title");
								
						}
						var jobstrim = jobs.slice(0,-1);
					    	if(jobstrim != oldjobs || x == 0 ){
	                                 
						       if(x!= 0){
		
							        var randomnumber = Math.floor(random()*54);
					 
						           	var oldcolor= document.getElementById(keydetail+(x-1)+nodename).getAttribute("bgcolor");
								while(oldcolor == ColorArray[randomnumber]){
									randomnumber = Math.floor(random()*54);
								}
						           		document.getElementById(keydetail+x+nodename).setAttribute("bgcolor",ColorArray[randomnumber]);
							}
							else 
							{
									var randomnumber = Math.floor(random()*54);
					        	   		document.getElementById(keydetail+x+nodename).setAttribute("bgcolor",ColorArray[randomnumber]);
						            	
							}
						}
					  	else{
						
							if(x!= 0){
								var oldcolor=   document.getElementById(keydetail+(x-1)+nodename).getAttribute("bgcolor");
						  		document.getElementById(keydetail+x+nodename).setAttribute("bgcolor",oldcolor);}
							else
						   	{
						  
									var randomnumber = Math.floor(random()*54);
							   		document.getElementById(keydetail+x+nodename).setAttribute("bgcolor",ColorArray[randomnumber]);
							   	
						   	}
                        	             	}
					}
			  
					if(jobs == "_")
				    		document.getElementById(keydetail+x+nodename).setAttribute("title","no jobs");
		        		else {
			 			var trimval = jobs.slice(0,-1);		
			    			document.getElementById(keydetail+x+nodename).setAttribute("title",trimval);
					}
				    });
	
				}
	
	 		});
       		        x++;
                
 		});
		
	});



});                




}



//===========================================================================================================
//
//	BUTTON ONCLICK FUNCTIONS 
//
//===========================================================================================================


//-------------------------------------------------------------
// About button onclick
//_____________________________________________________________

function aboutclick()
{

   document.getElementById("pagecontent").innerHTML = "<h2>About Us and SLURM</h2>";

}


//-------------------------------------------------------------
// Program button onclick
//_____________________________________________________________

function programclick()
{
	
document.getElementById("pagecontent").innerHTML = "<div id='tabs' ><ul>		<li><a href='#tabs-1' >CONTROLLER STATUS</a></li>	<li><a href='#tabs-2' >NODE STATUS</a></li><li><a href='#tabs-3' >JOB STATUS</a></li>	<li><a href='#tabs-5'>PARTITION STATUS</a></li>	<li><a href='#tabs-4' >REPORTS</a></li>	</ul>	<div id='tabs-1'></br></br><div id='tabs-1data'></div>	</div> <div id='tabs-2'></br><div><h3>Color Codes</h3> <table><tr><td  bgcolor='#FF0040'>&nbsp;&nbsp;&nbsp;&nbsp;</td><td> : Allocated&nbsp;&nbsp;</td><td  bgcolor='#80FF00'>&nbsp;&nbsp;&nbsp;&nbsp;</td><td> : Idle&nbsp;&nbsp;</td></tr><tr><td  bgcolor='#F5BCA9'>&nbsp;&nbsp;&nbsp;&nbsp;</td><td> : Down&nbsp;&nbsp;</td><td  bgcolor='#58FAF4'>&nbsp;&nbsp;&nbsp;&nbsp;</td><td> : Unknown&nbsp;&nbsp;</td></tr></table></div></br> <div id='tabs-2data'></div>  </div> <div id='tabs-3'></br><div><h3>Color Codes</h3> <table><tr><td  bgcolor='#FF0040'>&nbsp;&nbsp;&nbsp;&nbsp;</td><td> : Waiting&nbsp;&nbsp;</td><td  bgcolor='#80FF00'>&nbsp;&nbsp;&nbsp;&nbsp;</td><td> : Running&nbsp;&nbsp;</td></tr><tr><td  bgcolor='#F5BCA9'>&nbsp;&nbsp;&nbsp;&nbsp;</td><td> : Stopped&nbsp;&nbsp;</td></tr></table></div></br><div id='tabs-3data'></div>	</div><div id='tabs-5'></br></br><div id='tabs-5data'></div></div><div id='tabs-4'></br></br><div id='tabs-4data'></div></div></div>";

	$( "#tabs" ).tabs();
	$( "#tabs" ).css("height","2000");
	$( "#tabs" ).css("min-width","1000px");

}


//-------------------------------------------------------------
// Conctact button onclick
//_____________________________________________________________

function contactclick()
{

	document.getElementById("pagecontent").innerHTML = "<h2>Contact Information</h2>";

}

<!--cellpadding="0" cellspacing="0"--!>
 
//========
//  END
//========

