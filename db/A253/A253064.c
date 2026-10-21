/*
 * A253064  Number of odd terms in f^n, where f = 1/x + 1 + x + y.
 * https://oeis.org/A253064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over runs of 1-bits in binary n of * F_{k+2} ×
 *            2^{(# of 1-bits in n)}, * where k is the run length and F_j is
 *            the j-th Fibonacci number * (F_0 = 0, F_1 = 1). This is the Run
 *            Length Transform of * A087206 (basis sequence F_{k+2} for k≥1).
 *            The implementation * uses a small static tab...
 * Range:     u8 n <= 30, u16 n <= 1022, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build a small table of Fibonacci numbers on first call. */
    static int init = 0;
    static seq_t fib[67];   /* fib[i] = F_i for i = 0 .. 66 */

    if (!init) {
        fib[0] = 0;
        fib[1] = 1;
        for (idx_t i = 2; i <= 66; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        init = 1;
    }

    /* a(0) = 1 by definition (empty product). */
    if (n == 0)
        return 1;

    /* Product of the F_{len+2} for each run of consecutive 1's. */
    seq_t prodF = 1;
    idx_t runLen = 0;
    idx_t popcnt = 0;
    idx_t temp   = n;

    /* Scan bits from least‑significant to most‑significant. */
    while (temp) {
        if (temp & 1U) {          /* bit is 1: extend current run */
            ++runLen;
            ++popcnt;
        } else {                 /* bit is 0: run ends, multiply */
            if (runLen) {
                prodF *= fib[runLen + 2];
                runLen = 0;
            }
        }
        temp >>= 1U;
    }
    if (runLen)                  /* final run at MSB side */
        prodF *= fib[runLen + 2];

    /* Multiply by 2^{popcnt}. */
    seq_t result = prodF;
    for (idx_t i = 0; i < popcnt; ++i)
        result *= 2;

    return result;
}
