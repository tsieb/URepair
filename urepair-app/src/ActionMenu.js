import React from 'react';
import './App.css';
import JobForm from './JobForm';


function ActionMenu(props) {

    return (
       <div className='ActionMenu'>
          <div className='border'>
             <JobForm 
                addJob={props.addJob}
             />
          </div>
       </div>
    )
 }

 export default ActionMenu;