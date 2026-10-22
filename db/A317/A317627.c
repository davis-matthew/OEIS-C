/*
 * A317627  a(n) = A317253 - floor(8*n/3).
 * https://oeis.org/A317627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A317627
 * Defined by case‑wise recursion:
 *   a(6k+1) = 2
 *   a(6k+4) = 1
 *   a(6k+3) = 1 - 2 * ((k) mod 2)
 *   a(6k+2) = 3 - 2 * ((k) mod 2)
 *   a(6k+5) = a(3k+2)
 *   a(6k)   = a(n/2)          // for n ≥ 6
 *
 * This implementation uses only integer arithmetic and works for all
 * n ≥ 1.  It has O(log n) time complexity.
 */
seq_t a(idx_t n)
{
    while (1) {
        switch (n % 6) {
        case 0:             // n is divisible by 6
            n = n / 2;
            continue;

        case 1:             // n = 6k+1
            return (seq_t)2;

        case 4:             // n = 6k+4
            return (seq_t)1;

        case 3: {           // n = 6k+3
            idx_t k = n / 6;
            return (seq_t)((seq_t)1 - (seq_t)2 * (seq_t)(k & 1));
        }

        case 2: {           // n = 6k+2
            idx_t k = n / 6;
            return (seq_t)((seq_t)3 - (seq_t)2 * (seq_t)(k & 1));
        }

        case 5: {           // n = 6k+5   → reduce to a(3k+2)
            idx_t k = (n - 5) / 6;
            n = 3 * k + 2;
            continue;
        }
        }
    }
}
