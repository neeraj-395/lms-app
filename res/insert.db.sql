USE lms_db;

INSERT INTO users (name, password, email, phone) VALUES
('Priyanshu Garg', 'pass726', 'priyanshu@example.com', '1234567890'),
('Sudhanshu Kaul', 'pass436', 'sudhanshu@example.com', '2345678901'),
('Rahul K.Tiwari', 'pass408', 'rahul.k@example.com', '3456789012');


INSERT INTO librarians (name, password, email, phone) VALUES
('Neeraj Singh', 'libpass395', 'neeraj@example.com', '4567890123'),
('Vinayak', 'libpass456', 'vinayak@example.com', '5678901234');


INSERT INTO books (title, author, isbn, quantity) VALUES
('The Great Gatsby', 'F. Scott Fitzgerald', '9780743273565', 5),
('To Kill a Mockingbird', 'Harper Lee', '9780061120084', 2),
('1984', 'George Orwell', '9780451524935', 3),
('Pride and Prejudice', 'Jane Austen', '9781503290563', 4),
('Moby-Dick', 'Herman Melville', '9781503280786', 1);


INSERT INTO borrowers (book_id, user_id, checkout_date, return_date) VALUES
(1, 1, '2024-06-01', '2024-06-15'),
(2, 2, '2024-06-02', '2024-06-16'),
(3, 3, '2024-06-03', '2024-06-17');

INSERT INTO fines (borrower_id, fine_amount) VALUES
(1, 50.00),
(2, 69.00),
(3, 99.00);
