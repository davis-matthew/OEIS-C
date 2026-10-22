/*
 * A366901  The number of exponentially odious divisors of n.
 * https://oeis.org/A366901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = A115384(e)+1, where A115384(e) is
 *            the number of odious integers ≤ e. (An odious integer has an odd
 *            number of 1‑bits in its binary representation.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t odious_count[65];
static int odious_init = 0;

/* Count the 1‑bits of a 64‑bit integer (simple loop). */
static unsigned int popcnt64(uint64_t x)
{
    unsigned int c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

/* Initialise odious_count[0..64] on first use. */
static void init_odious_counts(void)
{
    if (odious_init) return;
    unsigned int c = 0;
    odious_count[0] = 0;
    for (idx_t i = 1; i <= 64; ++i) {
        if ((popcnt64((uint64_t)i) & 1) != 0) ++c;
        odious_count[i] = c;
    }
    odious_init = 1;
}

/* Compute a(n): the number of exponentially odious divisors of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* only used for safety */
    init_odious_counts();

    seq_t result = 1;
    idx_t tmp = n;

    /* handle factor 2 */
    idx_t exp = 0;
    while ((tmp & 1) == 0) {            /* tmp % 2 == 0 */
        tmp >>= 1;
        ++exp;
    }
    if (exp > 0) {
        result *= (seq_t)(odious_count[exp] + 1);
    }

    /* handle odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            result *= (seq_t)(odious_count[exp] + 1);
        }
    }

    /* remaining prime > 1 */
    if (tmp > 1) {
        result *= (seq_t)(odious_count[1] + 1);    /* exponent 1 */
    }

    return result;
}
