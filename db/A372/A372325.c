/*
 * A372325  Numbers whose binary expansion has an even number of 1's among
 *          positions listed in this sequence.
 * https://oeis.org/A372325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Definition: Let S be the set of non‑negative integers m such that
 * the number of exponents e (positions of 1 bits in the binary
 * representation of m) which belong to S itself is even.
 * a(n) is the n‑th element of S (offset = 1, so a(1)=0).
 *
 * This definition yields the recurrence
 *   x_m = 1 + Σ_{e∈E(m)} x_e   (mod 2),
 * where x_m = 1 if m∈S else 0 and E(m) is the set of bit positions of m.
 *
 * The implementation computes S incrementally: for each m the parity
 * of x_e over its set bit positions is accumulated, using only previously
 * computed values.  The sequence is then output in order.
 */

#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t types are supplied by the build environment. */

seq_t a(idx_t n)
{
    if (n == 0) {            /* undefined for offset 1, but return 0 safely */
        return (seq_t)0;
    }

    /* Store membership (0/1) for each computed integer. */
    unsigned char *mem = NULL;
    size_t mem_cap = 16384;          /* start with a reasonable size */
    mem = (unsigned char *)malloc(mem_cap * sizeof(unsigned char));
    if (!mem) return (seq_t)0;       /* allocation failure -> 0 */

    idx_t m = 0;
    idx_t count = 0;

    for (;;) {
        /* Ensure capacity for current m */
        if ((size_t)m >= mem_cap) {
            size_t new_cap = mem_cap * 2;
            unsigned char *tmp = (unsigned char *)realloc(mem, new_cap * sizeof(unsigned char));
            if (!tmp) { /* on failure, cleanup and return */
                free(mem);
                return (seq_t)0;
            }
            mem = tmp;
            mem_cap = new_cap;
        }

        /* Compute parity of x_e over set bits of m. */
        unsigned char parity = 0;
        idx_t temp = m;
        while (temp) {
            /* idx_t pos = __builtin_ctzll(temp);  // count trailing zeros */
            idx_t pos = (idx_t)__builtin_ctzll(temp);
            parity ^= mem[pos];
            temp &= temp - 1;              /* clear lowest set bit */
        }

        /* x_m = 1 + parity (mod 2) == 1 ^ parity */
        unsigned char mem_val = 1 ^ parity;
        mem[m] = mem_val;

        if (mem_val) {
            ++count;
            if (count == n) {
                free(mem);
                return (seq_t)m;
            }
        }

        ++m;
    }
}
