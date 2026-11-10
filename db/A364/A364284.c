/*
 * A364284  Numbers k such that A261627(k) = 1.
 * https://oeis.org/A364284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k with A261627(k) = 1; found by a sieve-based
 *            search over k (A261627 computed directly, early exit once count
 *            exceeds 1)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A364284_LIMIT 100000u

static uint32_t A364284_terms[64];
static int A364284_nterms = 0;
static uint32_t A364284_next = 1;
static bool A364284_done = false;
static unsigned char *A364284_comp = NULL;

static void A364284_init(void)
{
    uint32_t M = 2 * A364284_LIMIT + 2;
    A364284_comp = (unsigned char *)calloc(M + 1, 1);
    A364284_comp[0] = 1;
    A364284_comp[1] = 1;
    for (uint64_t i = 2; i * i <= M; i++)
        if (!A364284_comp[i])
            for (uint64_t j = i * i; j <= M; j += i)
                A364284_comp[j] = 1;
}

static bool A364284_good(uint32_t k)
{
    uint32_t kp = (k & 1) ? 1 : 2;
    int count = 0;
    for (uint32_t p = 2; (uint64_t)p * kp < k; p++) {
        if (A364284_comp[p]) continue;
        uint32_t t = p * kp;
        uint32_t left = k - t + 1;
        uint32_t right = k + t - 1;
        if (!A364284_comp[left] && !A364284_comp[right]) {
            if (++count > 1) return false;
        }
    }
    return count == 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!A364284_comp) A364284_init();
    while ((idx_t)A364284_nterms < n && !A364284_done) {
        if (A364284_next > A364284_LIMIT) { A364284_done = true; break; }
        uint32_t k = A364284_next++;
        if (A364284_good(k) && A364284_nterms < 64)
            A364284_terms[A364284_nterms++] = k;
    }
    if ((idx_t)A364284_nterms >= n) return (seq_t)A364284_terms[n - 1];
    return 0;
}
