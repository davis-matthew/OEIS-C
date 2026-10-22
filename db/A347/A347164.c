/*
 * A347164  Positive integers k such that the decimal representation of 2^k
 *          ends with some permutation of the string "0123456789".
 * https://oeis.org/A347164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A347164.
 *
 * An integer k is in the sequence iff the last ten digits of 2^k form a
 * permutation of the string "0123456789".  The residues of such k modulo
 * D = 7812500 (the period of 2^k modulo 10^10) form finite arithmetic
 * progressions; this routine finds the n‑th smallest element of the union
 * of those progressions.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 10^10 fits into 64 bits. */
static const uint64_t MOD = 10000000000ULL;

/* Period of 2^k modulo 10^10. */
static const uint32_t D32 = 7812500U;
static const uint64_t PERIOD = 7812500ULL;

/* Storage for residues r such that 2^r (mod 10^10) is a 0–9 permutation. */
static uint32_t *residues      = NULL;
static size_t    residues_cnt  = 0;
static size_t    residues_cap  = 0;
static bool      residues_set  = false;

/* ---------------------------------------------------------------------- */
/* Utility: push a new residue into the dynamic array.                   */
static void push_residue(uint32_t r)
{
    if (residues_cnt == residues_cap) {
        size_t new_cap = residues_cap ? residues_cap * 2 : 64;
        uint32_t *tmp = (uint32_t *)realloc(residues, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);
        residues = tmp;
        residues_cap = new_cap;
    }
    residues[residues_cnt++] = r;
}

/* ---------------------------------------------------------------------- */
/* Pre‑compute all residues 0 < r < PERIOD with 2^r mod 10^10 a digit
 * permutation.  The computation is done once on first call.           */
static void init_residues(void)
{
    if (residues_set) return;
    residues_set = true;

    uint64_t cur = 1;                      /* 2^0 mod 10^10 */
    uint64_t t;
    uint32_t cnt[10];

    for (uint32_t k = 1; k < D32; ++k) {
        /* 2^k = 2 * 2^(k-1) mod 10^10 */
        cur <<= 1;
        if (cur >= MOD) cur -= MOD;

        /* Count decimal digits of cur (treating leading zeros). */
        t = cur;
        for (int d = 0; d < 10; ++d) cnt[d] = 0;
        for (int d = 0; d < 10; ++d) {
            cnt[t % 10U] += 1;
            t /= 10U;
        }

        /* Check that each digit occurs exactly once. */
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (cnt[d] != 1U) { ok = false; break; }
        }
        if (ok) push_residue(k);
    }
}

/* ---------------------------------------------------------------------- */
/* Main sequence generator.                                               */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_residues();

    size_t m = residues_cnt;
    if (m == 0) return 0;                  /* defensive, should not happen */

    /* Array holding the current smallest term for each residue class. */
    uint64_t *current = (uint64_t *)malloc(m * sizeof(uint64_t));
    if (!current) exit(1);

    for (size_t i = 0; i < m; ++i) {
        uint64_t val = (uint64_t)residues[i];
        if (val <= 33ULL) val += PERIOD;   /* ensure first term > 33 */
        current[i] = val;
    }

    seq_t result = 0;
    for (idx_t iter = 1; iter <= n; ++iter) {
        /* Find minimal current value. */
        size_t min_idx = 0;
        uint64_t min_val = current[0];
        for (size_t i = 1; i < m; ++i) {
            if (current[i] < min_val) {
                min_val = current[i];
                min_idx = i;
            }
        }

        if (iter == n) {
            result = (seq_t)min_val;
            break;
        }

        /* Advance the chosen residue class by one period. */
        current[min_idx] += PERIOD;
    }

    free(current);
    return result;
}
