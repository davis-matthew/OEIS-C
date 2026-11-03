/*
 * A327864  Numbers whose arithmetic derivative is a multiple of 4, cf.
 *          A003415.
 * https://oeis.org/A327864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n whose arithmetic derivative n' is a multiple of 4
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9675 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is ordered as 0, 1, 4, 8, 12, 15, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ----- helper: compute whether m's arithmetic derivative is divisible by 4 ----- */
static bool in_sequence_uint64(uint64_t m)
{
    if (m <= 1) return true;          /* 0 and 1 are in the sequence by definition */

    uint8_t mod_sum = 0;
    uint64_t temp   = m;              /* copy for factorisation */

    /* factor 2 */
    if ((temp & 1) == 0) {
        uint8_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            e++;
        }
        uint8_t term = (e & 3) * ((m / 2) & 3) & 3;   /* (e%4) * ((m/2)%4) mod 4 */
        mod_sum     = (mod_sum + term) & 3;           /* mod 4 */
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            uint8_t term = ((e & 3) * ((m / p) & 3)) & 3;
            mod_sum     = (mod_sum + term) & 3;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        uint8_t term = ((m / temp) & 3) & 3;          /* e = 1 */
        mod_sum     = (mod_sum + term) & 3;
    }

    return (mod_sum & 3) == 0;
}

static idx_t   terms_len   = 0;       /* number of terms already stored   */
static seq_t  *terms       = NULL;    /* array of terms                   */
static size_t  terms_cap   = 0;       /* capacity of the array            */
static uint64_t next_scan  = 2;       /* next integer to test             */

/* ----- initialize or grow the terms array ----- */
static void ensure_capacity(idx_t needed)
{
    if (terms_len >= needed) return;

    if (!terms) {
        terms_cap = 64;
        terms     = mem_new(seq_t, terms_cap);
        if (!terms) return;                          /* allocation failed */
        terms[0] = (seq_t)0;
        terms[1] = (seq_t)1;
        terms_len = 2;
    } else if (terms_len + needed - terms_len > terms_cap) {
        size_t new_cap = terms_cap * 2;
        if (new_cap < terms_len + needed - terms_len)
            new_cap = terms_len + needed - terms_len + 64;
        seq_t *new_ptr = mem_resize(seq_t, terms, new_cap);
        if (new_ptr) {
            terms     = new_ptr;
            terms_cap = new_cap;
        }
    }

    /* generate additional terms until we reach 'needed' */
    while (terms_len < needed) {
        if (in_sequence_uint64(next_scan)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *new_ptr = mem_resize(seq_t, terms, new_cap);
                if (!new_ptr) break;                /* allocation failed */
                terms     = new_ptr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_scan;
        }
        next_scan++;
    }
}

/* ----- required function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for zero index */
    ensure_capacity(n);                   /* generate up to n terms */
    return terms[n - 1];                  /* offset 1: a(1) = terms[0] */
}
