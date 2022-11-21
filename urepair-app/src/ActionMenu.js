import React from 'react';
import './App.css';
import PostForm from './PostForm';


function ActionMenu(props) {

    return (
       <div className='ActionMenu'>
          <div className='border'>
             <PostForm 
                addJob={props.addJob}
             />
          </div>
       </div>
    )
 }

 export default ActionMenu;