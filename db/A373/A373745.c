/*
 * A373745  Maximum length of a run of alternating bits in the base-2
 *          representation of prime(n).
 * https://oeis.org/A373745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A374176(prime(n)) + 1 (maximum alternating‑bit run + 1)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;            /* undefined for n = 0, harmless */
    }

    /* obtain the nth prime (fits in 32 bits for n up to 10 000) */
    uint32_t p = get_prime(n);

    /* compute bit length of p */
    int bits = 0;
    uint32_t tmp = p;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }
    if (bits == 0) {
        return (seq_t)0;            /* should not happen for prime >= 2 */
    }

    /* find longest alternating‑bit run in the binary representation of p */
    int max_len = 1, cur_len = 1;
    int prev_bit = (int)((p >> (bits - 1)) & 1);

    for (int i = bits - 2; i >= 0; --i) {
        int bit = (int)((p >> i) & 1);
        if (bit != prev_bit) {
            cur_len++;
        } else {
            cur_len = 1;
        }
        if (cur_len > max_len) {
            max_len = cur_len;
        }
        prev_bit = bit;
    }

    return (seq_t)max_len;
}
