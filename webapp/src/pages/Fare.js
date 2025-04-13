import React, { useState, useEffect } from 'react';
import './Fare.css';
import busIcon from './icons/bus.png';
import refreshIcon from './icons/refresh.png';

const Fare = () => {
  const [trips, setTrips] = useState([]);
  const fetchTrips2 = async () => {
    try {
      // Replace these API endpoints with your actual endpoints
      const fareResponse = await fetch('https://api.thingspeak.com/channels/2324730/feeds.json');

      const fareData = await fareResponse.json();

      setTrips(fareData.feeds);
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  };
  useEffect(() => {
    // Simulate fetching data from API endpoints
    const fetchTrips = async () => {
      try {
        // Replace these API endpoints with your actual endpoints
        const fareResponse = await fetch('https://api.thingspeak.com/channels/2324730/feeds.json');

        const fareData = await fareResponse.json();

        setTrips(fareData.feeds);
      } catch (error) {
        console.error('Error fetching data:', error);
      }
    };

    // Call the fetchTrips function
    fetchTrips();
  }, []); // Empty dependency array means the effect runs once when the component mounts

  return (
    <div>
      {/* Navbar */}
      <nav className="navbar navbar-dark bg-dark">
        <span className="navbar-brand">
          <img src={busIcon} alt="Bus Icon" width="90" height="60" className="d-inline-block align-top" />
          {' Digital Fare Logging Portal'}
        </span>
      </nav>

      {/* Main Content */}
      <div className="fare-logging-container container mt-3">
        <ul className="list-group">
          {trips.map((trip, index) => (
            <li key={index} className="list-group-item bg-dark text-light">
              <strong>Trip {index + 1}:</strong>
              <br />
              Fare: {trip.field1}$
              <br />
              Distance: {trip.field2} miles
            </li>
          ))}
        </ul>
        <button className="btn btn-success mt-3" onClick={() => alert('Refresh button clicked')}>
          <img src={refreshIcon} onClick={fetchTrips2} alt="Refresh Icon" width="20" height="20" className="mr-2" />
          <br />Refresh Trips
        </button>
      </div>
    </div>
  );
};

export default Fare;
