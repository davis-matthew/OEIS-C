/*
 * A276867  First differences of the Beatty sequence A003231 for 2 + phi,
 *          where phi = golden ratio = (1 + sqrt(5))/2.
 * https://oeis.org/A276867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*r) - floor((n-1)*r), where r = (5 + sqrt5)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses scaled integer arithmetic: r ≈ (5+sqrt5)/2 ≈ 3.6180339887 */
/* Scale sqrt5 by 1e14 to keep sufficient precision for n up to 10000. */

static const unsigned long long SCALE = 100000000000000ULL;           /* 1e14  */
static const unsigned long long NUM   = 5 * SCALE + 223606797749979ULL;/* (5+sqrt5)*SCALE */

static inline seq_t floor_nr(idx_t n)
{
    /* floor(n*r) = floor(n*(5+sqrt5)/2) */
    unsigned __int128 tmp = (unsigned __int128)n * NUM;
    tmp /= (2ULL * SCALE);
    return (seq_t)tmp;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n = 0, harmless */
    seq_t fn  = floor_nr(n);
    seq_t fnm = floor_nr(n - 1);
    return fn - fnm;
}
