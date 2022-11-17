import React, { useState, useEffect } from 'react';
import axios from "axios";
import './App.css';


const client = axios.create({
   baseURL: "https://jsonplaceholder.typicode.com/posts"
});


function ActionMenu(props) {

   return(
      <div className='ActionMenu'>
         <div className='border'>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
         </div>
      </div>
   )
}

function ClientInfo(props) {

   return(
      <div className='ClientInfo'>
         <div className='border'>
            <header>
               Name
            </header>
            <h4>
               Contact info
            </h4>
         </div>
      </div>
   )
}


function ClientJobs(props) {
   const [title, setTitle] = useState('');
   const [body, setBody] = useState('');
   const [posts, setPosts] = useState([]);

   // GET with Axios
   useEffect(() => {
      const fetchPost = async () => {
         let response = await client.get('?_limit=20');
         setPosts(response.data);
      };
      fetchPost();
   }, []);

   // Delete with Axios
   const deletePost = async (id) => {
      await client.delete(`${id}`);
      setPosts(
         posts.filter((post) => {
            return post.id !== id;
         })
      );
   };

   // Post with Axios
   const addPosts = async (title, body) => {
      let response = await client.post('', {
         title: title,
         body: body,
      });
      setPosts((posts) => [response.data, ...posts]);
   };

   const handleSubmit = (e) => {
      e.preventDefault();
      addPosts(title, body);
   };


   return (
      <div className="ClientJobs">
         <div className='border'>
            {posts.map((post) => {
               return (
                  <div className="post-card" key={post.id}>
                     #{post.id} - {post.title} 
                     <div className="button">
                        <div className="delete-btn" onClick={() => deletePost(post.id)}>
                           {post.body}
                        </div>
                     </div>
                  </div>
               );
            })}
         </div>
      </div>

   )



}



function App() {
   return (
      <div className='App'>
         <div id='appHeader'>
            Logo and top bar
         </div>
         <div id='leftCol'>
            <ActionMenu/>
         </div>
         <div id='centreCol'>
            <ClientJobs/>
         </div>
         <div id='rightCol'>
            <ClientInfo/>
         </div>
      </div>
   );
};

export default App;
