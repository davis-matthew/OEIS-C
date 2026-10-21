/*
 * A264105  a(n) = smallest k such that n divides Sum_{i=1..k} Fibonacci(i).
 * https://oeis.org/A264105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 194, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest k such that n divides Sum_{i=1..k} Fibonacci(i)
   (i.e., F(k+2)-1 is divisible by n).
   The function iterates Fibonacci numbers modulo n until a value
   congruent to 1 modulo n is found. The corresponding k is j-2
   where j is the index of that Fibonacci number. */
seq_t a(idx_t n) {
    /* Special case for n = 1: any k works, so the smallest is 1. */
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t mod = (seq_t)n;
    seq_t a_val = (seq_t)1;   /* F(1) mod n */
    seq_t b_val = (seq_t)1;   /* F(2) mod n */

    /* start from j = 3, which corresponds to k = 1 */
    for (idx_t j = 3;; ++j) {
        seq_t c = (a_val + b_val) % mod; /* F(j) mod n */
        a_val = b_val;
        b_val = c;

        if (b_val == (seq_t)1) { /* F(j) ≡ 1 (mod n) */
            return (seq_t)(j - 2); /* k = j - 2 */
        }
    }
}
