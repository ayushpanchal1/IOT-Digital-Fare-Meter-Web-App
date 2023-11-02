import logo from './logo.svg';
import './App.css';
import { BrowserRouter, Routes, Route,} from 'react-router-dom';
import Fare from './pages/Fare'

function App() {
  return (
    <div>
      <BrowserRouter>
        <Routes>
          <Route path="/" exact element={<Fare/>} />
        </Routes>
      </BrowserRouter>
        
    </div>
  );
}

export default App;
