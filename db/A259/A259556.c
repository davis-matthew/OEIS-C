/*
 * A259556  Rectangular array, read by antidiagonals: T(h,k) = u(h) + v(k),
 *          where u = A000201 (lower Wythoff numbers), v = A001950 (upper
 *          Wythoff numbers), and h >= 1, k >= 1.
 * https://oeis.org/A259556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = fstWythoff(h) + fstWythoff(k) + k where (h,k) are
 *            anti‑diagonal coordinates of n: Let d be the smallest integer
 *            with (d−1)d/2 ≥ n. offset = n − (d−2)(d−1)/2 − 1 (0‑based) h =
 *            offset + 1; k = d − h
 * Range:     u8 n <= 1830, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(√x) for 128‑bit unsigned integers. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    for (int i = 63; i >= 0; --i) {
        unsigned __int128 t = r | ((unsigned __int128)1 << i);
        if (t <= x / t)
            r = t;
    }
    return r;
}

/* Return u(n) = ⌊n·φ⌋ where φ = (1+√5)/2, using integer arithmetic. */
static seq_t lower_wythoff(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    unsigned __int128 hn = (unsigned __int128)n;
    unsigned __int128 val = hn * hn * (unsigned __int128)5; /* 5·n² */
    unsigned __int128 sqrt_val = isqrt128(val);              /* ⌊√(5·n²)⌋ = ⌊n√5⌋ */
    unsigned __int128 sum   = hn + sqrt_val;                 /* n + ⌊n√5⌋ */
    unsigned __int128 res   = sum >> 1;                       /* floor((n + ⌊n√5⌋)/2) = ⌊n·φ⌋ */
    return (seq_t)res;
}

/* Convert n (≥1) to anti‑diagonal coordinates (h,k). */
static void antidiagonal_coords(idx_t n, idx_t *h, idx_t *k)
{
    /* Find d such that (d−1)d/2 ≥ n. */
    idx_t low  = 2, high = 2;
    while ( ((unsigned __int128)(high - 1) * high) / 2 < n )
        high <<= 1;                      /* exponential search */

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( ((unsigned __int128)(mid - 1) * mid) / 2 >= n )
            high = mid;
        else
            low = mid;
    }
    idx_t d = high;

    unsigned __int128 prev = ((unsigned __int128)(d - 2) * (d - 1)) / 2; /* elements before diag d */
    idx_t offset = (idx_t)( (unsigned __int128)n - prev - 1 );        /* 0‑based within diagonal */

    *h = offset + 1;
    *k = d - *h;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t h, k;
    antidiagonal_coords(n, &h, &k);

    seq_t u_h = lower_wythoff(h);
    seq_t u_k = lower_wythoff(k);

    return u_h + u_k + (seq_t)k;
}
