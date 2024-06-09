# MongoDB Clone Project

Welcome to the MongoDB Clone Project! This project aims to provide a clone of MongoDB with similar query methods and functionality. Users can perform CRUD operations using MongoDB-like queries, and data is stored using a modified B-tree structure and BSON format.

## Features

- Perform CRUD operations (Create, Read, Update, Delete) using MongoDB-like queries.
- Supports both JSON and BSON file formats.
- Modified B-tree structure for data storage.
- Indexing support for efficient querying.
- Database management commands similar to MongoDB.

## Installation

Clone the repository to your local machine:

```bash
git clone https://github.com/your_username/mongodb-clone.git
```
## Getting Started

Follow these instructions to get started with the MongoDB Clone Project on your local machine.

### Prerequisites

Before you begin, make sure you have the following installed:

- C++ compiler
- Windows OS (for the provided code, can be modified for other platforms)

### Installation

Clone the repository to your local machine:

```bash
git clone https://github.com/your_username/mongodb-clone.git
```
## Navigate to the project directory:

cd mongodb-clone


## Building the Project

Compile the source files using your C++ compiler.

## Running the Project

Execute the compiled binary to start the server.

./mongodb-clone


## Usage

Once the server is running, you can perform CRUD operations using MongoDB-like queries.

## Features

- Supports CRUD operations
- Uses B-tree for data storage
- Supports JSON and BSON file formats
- Provides indexing capabilities
- Allows inserting both JSON and BSON files
- Includes a converter for file formats

## Commands

### Database Operations
- `load dbs`: Load available databases
- `show dbs`: Show available databases
- `use dbname`: Switch to a specific database (e.g., `use Pharma`)

### CRUD Operations
- `db.collection.find()`: Find documents in a collection (e.g., `db.users.find()`)
- `db.collection.findOne()`: Find a single document in a collection (e.g., `db.contacts.findOne({...})`)
- `db.collection.insert()`: Insert documents into a collection (e.g., `db.products.insert({...})`)
- `db.collection.updateOne()`: Update a single document in a collection (e.g., `db.orders.updateOne({...})`)
- `db.collection.updateMany()`: Update multiple documents in a collection (e.g., `db.customers.updateMany({...})`)
- `db.collection.deleteOne()`: Delete a single document from a collection (e.g., `db.contacts.deleteOne({...})`)
- `db.collection.deleteMany()`: Delete multiple documents from a collection (e.g., `db.tasks.deleteMany({...})`)

### Other Operations
- `db.create_index()`: Create an index on a collection (e.g., `db.users.create_index({...})`)
- `db.drop_index()`: Drop an index from a collection (e.g., `db.products.drop_index({...})`)



## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

## License

This project is licensed under the MIT License.



