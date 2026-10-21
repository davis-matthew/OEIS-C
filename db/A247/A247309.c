/*
 * A247309  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,1) to (n,k), where 0 <= k <= 2, consisting of
 *          segments given by the vectors (1,1), (1,0), (1,-1), (1,-2).
 * https://oeis.org/A247309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = F(2*⌊n/3⌋ – 1) if n ≡ 0 (mod 3), and F(2*⌊n/3⌋)
 *            otherwise, where F(0)=0, F(1)=1 and F(–1)=1
 * Range:     u8 n <= 21, u16 n <= 38, u32 n <= 72, u64 n <= 141,
 *            u128 n <= 281, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- fast doubling Fibonacci implementation --------------------------------- */
struct fp { seq_t f; seq_t fs; };    /* pair (F(n),F(n+1))                               */

static struct fp fib_pair(idx_t n)
{
    if (n == 0)
        return (struct fp){ (seq_t)0, (seq_t)1 };

    struct fp p = fib_pair(n >> 1);
    seq_t a = p.f,  b = p.fs;

    /* c = F(2k)   = F(k) * (2*F(k+1) – F(k))      */
    /* d = F(2k+1) = F(k)^2 + F(k+1)^2              */
    seq_t c = a * ((b << 1) - a);
    seq_t d = a * a + b * b;

    if ((n & 1) == 0)
        return (struct fp){ c, d };
    else
        return (struct fp){ d, c + d };
}

static seq_t fib_seq(idx_t n)
{
    return fib_pair(n).f;
}

/* --- sequence a(n) implementation ---------------------------------------- */
seq_t a(idx_t n)
{
    idx_t q = n / 3;   /* column number */
    idx_t r = n % 3;   /* row offset   */

    /* get the Fibonacci index k */
    if (r == 0) {
        if (q == 0)           /* k = –1: F(–1)=1 */
            return (seq_t)1;
        return fib_seq(2 * q - 1);
    }
    return fib_seq(2 * q);
}
