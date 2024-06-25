USE lms_db;

-- Insert demo data into users table
INSERT INTO users (name, password, email, phone) VALUES
('Priyanshu Garg', 'pass726', 'priyanshu@example.com', '123-456-7890'),
('Sudhanshu Kaul', 'pass436', 'sudhanshu@example.com', '234-567-8901'),
('Rahul K.Tiwari', 'pass408', 'rahul.k@example.com', '345-678-9012');

-- Insert demo data into librarians table
INSERT INTO librarians (name, password, email, phone) VALUES
('Neeraj Singh', 'libpass395', 'neeraj@example.com', '456-789-0123'),
('Vinayak', 'libpass456', 'vinayak@example.com', '567-890-1234');

-- Insert demo data into books table
INSERT INTO books (title, author, isbn) VALUES
('The Great Gatsby', 'F. Scott Fitzgerald', '9780743273565'),
('To Kill a Mockingbird', 'Harper Lee', '9780061120084'),
('1984', 'George Orwell', '9780451524935'),
('Pride and Prejudice', 'Jane Austen', '9781503290563'),
('Moby-Dick', 'Herman Melville', '9781503280786');

-- Insert demo data into borrowers table
INSERT INTO borrowers (book_id, user_id, checkout_date, return_by) VALUES
(1, 1, '2024-05-01', '2024-05-15'),
(2, 2, '2024-05-02', '2024-05-16'),
(3, 3, '2024-05-03', '2024-05-17');

-- Insert demo data into fines table
INSERT INTO fines (borrower_id, fine_amount) VALUES
(1, 50.00),
(2, 100.00),
(3, 99.00);  -- No fine for the third borrower

-- Verify insertions
SELECT * FROM users;
SELECT * FROM librarians;
SELECT * FROM books;
SELECT * FROM borrowers;
SELECT * FROM fines;