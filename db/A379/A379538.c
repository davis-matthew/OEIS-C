/*
 * A379538  Square array read by ascending antidiagonals: T(n,k) is the k-th
 *          frugal number in base n.
 * https://oeis.org/A379538
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   minimal_x_for_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the first element of the square array read by ascending
    antidiagonals whose rows are the frugal numbers of each base.
    For index n (offset 2) we find the pair (base,k) that appears
    at position n, then compute the k‑th frugal number in that base.
    The frugal number is defined as a number that has more digits in
    its base‑n representation than the total number of digits of its
    prime factorisation (exponents counted when >1). 1 is always
    considered frugal.  */

/*  Implementation uses a small on‑the‑fly search for the k‑th
    frugal number.  For the required range (n up to about 10 000)
    this is easily fast enough.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/minimal_x_for_m.h"

/* Count digits of a non‑negative integer x in base b. */
static inline unsigned int digits_u64(uint64_t x, seq_t base)
{
    unsigned int d = 0;
    unsigned long long b = (unsigned long long)base;
    do {
        x /= b;
        ++d;
    } while (x);
    return d;
}

/* Return the k‑th frugal number (1‑based) in the given base. */
static seq_t kth_frugal(seq_t base, idx_t k)
{
    idx_t count = 0;
    uint64_t m = 1;   /* candidates start at 1 */
    for (;;) {
        unsigned len_m = digits_u64(m, base);

        /* Factorisation of m */
        uint64_t t = m;
        unsigned tot = 0;

        /* factor 2 */
        if ((t & 1ull) == 0) {
            uint64_t exp = 0;
            while ((t & 1ull) == 0) {
                t >>= 1;
                ++exp;
            }
            tot += digits_u64(2, base);
            if (exp > 1)
                tot += digits_u64(exp, base);
        }

        /* odd prime factors */
        for (uint64_t p = 3; p <= t / p; p += 2) {
            if (t % p == 0) {
                uint64_t exp = 0;
                while (t % p == 0) {
                    t /= p;
                    ++exp;
                }
                tot += digits_u64(p, base);
                if (exp > 1)
                    tot += digits_u64(exp, base);
            }
        }

        if (t > 1)   /* remaining prime factor > 1 */
            tot += digits_u64(t, base);

        if (len_m > tot) {
            ++count;
            if (count == k)
                return (seq_t)m;
        }
        ++m;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined below offset */

    /* Convert the offset to the (base,k) pair. */
    idx_t offset = n - 2;          /* zero‑based offset in the list    */
    idx_t m = offset + 1;          /* rank to use in triangular test   */
    idx_t x = minimal_x_for_m(m);  /* x = S-2                          */
    idx_t S = x + 2;               /* current antidiagonal sum          */

    /* number of entries before this antidiagonal      */
    idx_t cum_before = (x - 1) * x / 2;
    idx_t pos = offset - cum_before;      /* position within the antidiagonal */

    idx_t base = (S - 1) - pos;   /* base of the element                */
    idx_t k = S - base;           /* k-th frugal number in that base    */

    return kth_frugal((seq_t)base, k);
}
