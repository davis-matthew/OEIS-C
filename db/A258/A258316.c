/*
 * A258316  T(n,k)=Number of (n+1)X(k+1) 0..1 arrays with each 2X2 subblock
 *          having clockwise pattern 0000 or 0011.
 * https://oeis.org/A258316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(n1+3) + Fibonacci(n2+3) – 1, * where (n1,n2)
 *            are obtained from the Cantor anti‑diagonal * enumeration of the
 *            array T(n,k) = Fib(n+3)+Fib(k+3)-1
 * Range:     u8 n <= 55, u16 n <= 231, u32 n <= 990, u64 n <= 1104,
 *            u128 n <= 1104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The table is indexed by positive integers n,k ≥ 1.  
 * Enumerating the pairs (n,k) by increasing sum n+k and, for each
 * sum, by increasing n gives the sequence
 *  (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),….
 * Each pair is inserted into the sequence a(·) in that order.  */

/* Compute Fibonacci numbers in seq_t. The value m is an
 * idx_t (unsigned), usually small (≲200 for the required n). */
static seq_t fib_idx(idx_t m)
{
    if (m == 0)          return 0;
    if (m == 1)          return 1;
    seq_t a = 0;
    seq_t b = 1;
    for (idx_t i = 2; i <= m; ++i) {
        seq_t next = a + b;
        a = b;
        b = next;
    }
    return b;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the anti‑diagonal number s such that
     * 1 + 2 + … + s  ≥  n   (diagonal lengths are 1,2,3,…). */
    idx_t s = 1;
    while (s * (s + 1) / 2 < n)
        ++s;

    /* Number of terms before this diagonal. */
    idx_t prev = s * (s - 1) / 2;

    /* Offset of n within the current diagonal (0‑based). */
    idx_t offset = n - prev - 1;
    idx_t n1 = 1 + offset;       /* row index */
    idx_t n2 = (s + 1) - n1;     /* column index */

    /* Compute T(n1,n2) = Fib(n1+3)+Fib(n2+3)-1. */
    seq_t fn = fib_idx(n1 + 3);
    seq_t fk = fib_idx(n2 + 3);
    return fn + fk - 1;
}
