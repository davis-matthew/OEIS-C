/*
 * A374949  Michael Kwok's arithmetic progression of 27 primes.
 * https://oeis.org/A374949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 605185576317848261 + (n-1)*34661666592412860
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Arithmetic progression of 27 primes (OEIS A374949) */

static const seq_t start_value = (seq_t)605185576317848261ULL;
static const seq_t step       = (seq_t)34661666592412860ULL;

seq_t a(idx_t n)
{
    /* Return the n-th term for n >= 1; behaviour for n < 1 is irrelevant. */
    return start_value + (seq_t)(n - 1) * step;
}
