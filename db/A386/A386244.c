/*
 * A386244  Number of iterations of seven segments count x -> A277116(x) to go
 *          from n to a fixed point.
 * https://oeis.org/A386244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A386244
   a(n) = number of iterations of the seven‑segment count
   A277116(x) until x becomes 4, 5, or 6 (the only fixed points). */

/* seven‑segment counts for decimal digits; note that digit 9
   uses 5 segments (the bottom segment is omitted). */
static const seq_t seg[10] = { (seq_t)6, (seq_t)2, (seq_t)5, (seq_t)5, (seq_t)4,
                               (seq_t)5, (seq_t)6, (seq_t)3, (seq_t)7, (seq_t)5 };

/* Count the seven‑segment representation of a nonnegative number. */
static seq_t segcount(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)6;   /* 0 uses 6 segments */
    seq_t sum = 0;
    while (n > (seq_t)0)
    {
        int digit = (int)(n % (seq_t)10);
        sum += seg[digit];
        n /= (seq_t)10;
    }
    return sum;
}

/* Return the value of A386244 at index n. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* Immediate fixed points */
    if (x == (seq_t)4 || x == (seq_t)5 || x == (seq_t)6) return (seq_t)0;

    seq_t steps = 0;
    while (1)
    {
        x = segcount(x);
        ++steps;
        if (x == (seq_t)4 || x == (seq_t)5 || x == (seq_t)6)
            break;
    }
    return steps;
}
