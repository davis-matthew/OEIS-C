/*
 * A249072  Sum of prime indices in the prime factorization of A249337(n),
 *          counted with multiplicity: a(n) = A056239(A249337(n)).
 * https://oeis.org/A249072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(A249337(n)); A249337(1)=1, A249337(2)=2,
 *            A249337(n) = #{k<n-1... : A056239(A249337(k)) =
 *            A056239(A249337(n-1)), 1<=k<=n-1}; computed by direct simulation
 *            with a sieve
 * Range:     u8 n <= 12580, u16 n <= 12580, u32 n <= 12580, u64 n <= 12580,
 *            u128 n <= 12580, big any n
 * Validated: standard, pass, 114 terms through n = 9750 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A249072_S = NULL;   /* A056239 table */
static uint64_t *A249072_cnt = NULL; /* count of terms by A056239 value */
static uint64_t A249072_lim = 0;
static uint64_t *A249072_T = NULL;   /* A249337 terms */
static uint64_t *A249072_SS = NULL;  /* their A056239 values */
static size_t A249072_len = 0, A249072_cap = 0;

static void A249072_ensure(uint64_t need)
{
    if (need <= A249072_lim) return;
    uint64_t L = need * 2 + 16;
    uint64_t *S = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    uint64_t *spf = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    uint64_t *cnt = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    uint64_t idx = 0;
    for (uint64_t i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            ++idx;
            S[i] = idx;
            spf[i] = i;
            if (i * i <= L)
                for (uint64_t j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = i;
        } else {
            uint64_t p = spf[i];
            S[i] = S[i / p] + S[p];
        }
    }
    for (size_t k = 0; k < A249072_len; ++k) cnt[A249072_SS[k]]++;
    free(spf);
    free(A249072_S);
    free(A249072_cnt);
    A249072_S = S;
    A249072_cnt = cnt;
    A249072_lim = L;
}

static void A249072_push(uint64_t t)
{
    if (A249072_len == A249072_cap) {
        A249072_cap = A249072_cap ? A249072_cap * 2 : 64;
        A249072_T = (uint64_t *)realloc(A249072_T, A249072_cap * sizeof(uint64_t));
        A249072_SS = (uint64_t *)realloc(A249072_SS, A249072_cap * sizeof(uint64_t));
    }
    A249072_ensure(t + A249072_len + 2);
    uint64_t s = A249072_S[t];
    A249072_T[A249072_len] = t;
    A249072_SS[A249072_len] = s;
    ++A249072_len;
    A249072_cnt[s]++;
}

seq_t a(idx_t n)
{
    if (A249072_len == 0) {
        A249072_push(1);
        A249072_push(2);
    }
    while (A249072_len < (size_t)n) {
        uint64_t t = A249072_cnt[A249072_SS[A249072_len - 1]];
        A249072_push(t);
    }
    return (seq_t)A249072_SS[n - 1];
}
