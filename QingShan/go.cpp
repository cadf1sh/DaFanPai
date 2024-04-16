#include "go.h"

void go()
{
    //获取基础图像
//    memcpy(&imgGray[0],&mt9v03x_image[30],sizeof(imgGray));

    getExiacn();

    if (new_get_start_point())//先找起点
    {
       Along_Wall();
//       if(indentify_l_boundary(3)!=l_identify_flag)
//       {
//           ips114_show_string(0,110 ,"err");
//       }
//       else
//       {
//           ips114_show_string(0,110 ,"true");
//       }
//       indentify_r_boundary(3);
//       ips114_show_int(190,  20 , l_identify_flag, 3);
//       ips114_show_int(190,  40 , r_identify_flag, 3);
//       find_mid_line_err();
//       if( midline_exist_flag==1)
//       {
//            for(int i =0 ;i<my_row;i++)
//            {
//           ips114_draw_point(l_border[i],   i, RGB565_RED);//显示起点
//           ips114_draw_point(r_border[i],   i, RGB565_BLUE );//显示起点
//           ips114_draw_point(center_line[i],i, RGB565_GREEN);//显示起点
//            }
//       }
    }
//    获取基础图像



    //元素处理



    //元素处理
}
