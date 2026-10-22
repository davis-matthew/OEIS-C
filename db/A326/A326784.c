/*
 * A326784  BII-numbers of regular set-systems.
 * https://oeis.org/A326784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th BII‑number that generates a regular set–system
 *            according to the definition in OEIS A326784. No closed form is
 *            known; we simply test successive integers until the n‑th regular
 *            number is found
 * Range:     u8 n <= 47, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return the number of bits needed to represent x (2‑based index). */
static idx_t bitlen_seq_t_local(seq_t x)
{
    idx_t len = 0;
    while (x != (seq_t)0) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Return true if val is a regular BII‑number. */
static bool is_regular(seq_t val)
{
    /* Determine maximum vertex index that can appear. */
    idx_t max_bits = bitlen_seq_t_local(val);
    /* freq[0] is unused; indices run from 1 to max_bits. */
    size_t cap = (size_t)max_bits + 1;
    unsigned long long *freq = (unsigned long long*)calloc(cap, sizeof(unsigned long long));
    if (!freq) return false;              /* out of memory – unlikely */

    seq_t tmp   = val;
    idx_t bitpos = 1;                     /* position of least significant 1 */

    while (tmp != (seq_t)0) {
        if ((tmp & (seq_t)1) != (seq_t)0) {
            seq_t sub   = (seq_t)bitpos;
            idx_t subpos = 1;
            /* Count vertices of the binary index itself. */
            while (sub != (seq_t)0) {
                if ((sub & (seq_t)1) != (seq_t)0) {
                    ++freq[subpos];
                }
                sub >>= 1;
                ++subpos;
            }
        }
        tmp >>= 1;
        ++bitpos;
    }

    unsigned long long base = 0;
    for (idx_t i = 1; i < (idx_t)cap; ++i) {
        unsigned long long f = freq[i];
        if (f != 0) {
            if (base == 0) base = f;
            else if (f != base) {
                free(freq);
                return false;                /* not regular */
            }
        }
    }

    free(freq);
    return true;                          /* regular */
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)0;
    idx_t found = 0;
    while (found < n) {
        if (is_regular(candidate)) {
            ++found;
            if (found == n)  /* nth regular number found */
                return candidate;
        }
        ++candidate;
    }
    /* Should never reach here */
    return candidate;
}
