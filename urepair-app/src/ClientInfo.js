import React from 'react';
import './App.css';

function ClientInfo(props) {

   return (
      <div className='ClientInfo'>
         <div className='border'>
            <div className="name">
               Name standin
            </div>
            <div className='uid'>
               {props.uid}
            </div>
         </div>
      </div>
   )
}

 export default ClientInfo;