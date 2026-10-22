/*
 * A349537  Least positive integer m such that the n numbers 33*k^2*(k^3+1) (k
 *          = 1..n) are pairwise distinct modulo m.
 * https://oeis.org/A349537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349537: least positive integer m such that the n numbers
 * 33*k^2*(k^3+1) (k = 1..n) are pairwise distinct modulo m. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The trivial case n = 1 */
    if (n == (idx_t)1) return (seq_t)1;

    /* A static buffer reused for each trial modulus m.
       It grows only when a larger m must be tested. */
    static uint8_t *seen = NULL;   /* storage of used residues */
    static size_t seen_cap = 0;    /* current capacity of seen   */

    /* Minimal possible modulus is n (at least n distinct residues). */
    for (idx_t m = n; ; ++m) {
        /* Ensure the buffer can hold all residues modulo m. */
        if ((size_t)m > seen_cap) {
            uint8_t *tmp = (uint8_t *)realloc(seen, (size_t)m);
            if (!tmp) {
                free(seen);          /* in an unlikely out‑of‑memory case */
                return (seq_t)0;     /* error indicator */
            }
            seen     = tmp;
            seen_cap = (size_t)m;
        }

        /* Clear the buffer for the current candidate m. */
        memset(seen, 0, (size_t)m);

        bool ok = true;
        /* Check pairwise distinctness of the values modulo m. */
        for (idx_t k = 1; k <= n; ++k) {
#if defined(__SIZEOF_INT128__)
            unsigned __int128 km   = k;
            unsigned __int128 k2   = (km * km) % m;          /* k^2 mod m */
            unsigned __int128 k3   = (k2 * km) % m;          /* k^3 mod m */
            unsigned __int128 term = (33ULL * k2) % m;       /* 33 * k^2 mod m */
            unsigned __int128 val  = (term * (k3 + 1)) % m;  /* full polynomial mod m */
            size_t idx = (size_t)val;
#else
            /* Fallback for builds where __int128 is not available.
               Uses 64‑bit intermediate; acceptable because m < 2^64. */
            uint64_t km   = k % m;
            uint64_t k2   = (uint64_t)(((__uint128_t)km * km) % m);
            uint64_t k3   = (uint64_t)(((__uint128_t)k2 * km) % m);
            uint64_t term = (uint64_t)(((__uint128_t)33 * k2) % m);
            uint64_t val  = (uint64_t)(((__uint128_t)term * ((k3 + 1) % m)) % m);
            size_t idx = (size_t)val;
#endif
            if (seen[idx]) {          /* duplicate residue – m unsuitable */
                ok = false;
                break;
            }
            seen[idx] = 1;            /* mark this residue as used */
        }

        if (ok) {                    /* minimal m found */
            return (seq_t)m;
        }
    }
}
