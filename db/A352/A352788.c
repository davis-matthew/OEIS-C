/*
 * A352788  Squares in A213544.
 * https://oeis.org/A352788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k (in increasing order) such that A213544(m) = k is
 *            a perfect square, where A213544(m) = 1 + Sum_{j=2..m}
 *            j*phi(j)/2; computed by a running sum with a totient sieve,
 *            doubling the range as needed
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-10)
 * Helpers:   is_square, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_square.h"

static seq_t *sq_terms = NULL;
static size_t sq_count = 0, sq_cap = 0;
static uint64_t sq_m = 1;
static seq_t sq_S;
static bool sq_init = false;

static void sq_push(seq_t v)
{
    if (sq_count == sq_cap) {
        size_t nc = sq_cap ? sq_cap * 2 : 16;
        if (sq_terms == NULL) sq_terms = mem_new(seq_t, nc);
        else sq_terms = mem_resize(seq_t, sq_terms, nc);
        sq_cap = nc;
    }
    sq_terms[sq_count++] = v;
}

seq_t a(idx_t n)
{
    if (!sq_init) {
        sq_init = true;
        sq_S = 1;
        sq_push(sq_S);
    }
    while (sq_count < (size_t)n) {
        uint64_t L = sq_m * 2;
        if (L < 1024) L = 1024;
        uint32_t *phi = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
        if (!phi) return 0;
        for (uint64_t i = 0; i <= L; ++i) phi[i] = (uint32_t)i;
        for (uint64_t p = 2; p <= L; ++p) {
            if (phi[p] == p) {
                for (uint64_t j = p; j <= L; j += p)
                    phi[j] = phi[j] / (uint32_t)p * (uint32_t)(p - 1);
            }
        }
        for (uint64_t k = sq_m + 1; k <= L; ++k) {
            sq_S += (seq_t)k * (seq_t)phi[k] / 2;
            sq_m = k;
            if (is_square(sq_S)) {
                sq_push(sq_S);
                if (sq_count >= (size_t)n) break;
            }
        }
        free(phi);
    }
    return sq_terms[n - 1];
}
