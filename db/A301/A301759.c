/*
 * A301759  Semiperiods of the Fibonacci sequence mod n.
 * https://oeis.org/A301759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the semiperiod of the Fibonacci sequence modulo n.
   The semiperiod is the smallest k ≥ 1 such that
   Fib(k) ≡ 0 (mod n) and Fib(k+1) ≡ ±1 (mod n).  This is the
   definition of sequence A301759. */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1. */
    if (n <= 1)
        return (seq_t)1;

    idx_t a_val = 0;          /* Fib(0) mod n */
    idx_t b_val = 1 % n;      /* Fib(1) mod n */

    for (idx_t k = 1; ; ++k)
    {
        /* Compute Fib(k+1) mod n safely using 128‑bit addition. */
        idx_t c = (idx_t)(((unsigned __int128)a_val + b_val) % n);
        a_val = b_val;
        b_val = c;

        /* Check the semiperiod condition */
        if (a_val == 0 && (b_val == 1 || b_val == n - 1))
            return (seq_t)k;
    }
}
