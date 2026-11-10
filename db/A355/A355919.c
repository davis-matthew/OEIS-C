/*
 * A355919  Let n = p_1*p_2*...*p_k be the prime factorization of n, with the
 *          primes sorted in descending order; let b(n) = 7^(p_1 - 1)*13^(p_2
 *          - 1)*19^(p_3 - 1)*...*A002476(k)^(p_k - 1). Sequence lists m such
 *          that b(m) > A343771(m).
 * https://oeis.org/A355919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th m such that b(m) > A343771(m), where b(m) = Prod_i
 *            A002476(i)^(p_i-1) over the primes p_1>=p_2>=... of m, and
 *            A343771(m) is the least product of q_i^(f_i-1) over
 *            factorizations m = f_1*f_2*... with f_1>=f_2>=... and q_i the
 *            i-th prime == 1 (mod 3). Computed by a branch-and-bound searc...
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

static long double A355919_lq[64];
static bool A355919_ready = false;

static void A355919_init(void)
{
	int c = 0;
	for (uint64_t p = 7; c < 64; p += 6) {
		bool pr = true;
		for (uint64_t d = 2; d * d <= p; d++)
			if (p % d == 0) { pr = false; break; }
		if (pr) A355919_lq[c++] = logl((long double)p);
	}
	A355919_ready = true;
}

static void A355919_dfs(const uint64_t *divs, int nd, uint64_t rem, uint64_t maxf,
                        int idx, long double cur, long double *best, bool *found)
{
	if (rem == 1) {
		if (cur < *best) { *best = cur; *found = true; }
		return;
	}
	if (idx >= 64) return;
	for (int i = nd - 1; i >= 1; i--) {
		uint64_t d = divs[i];
		if (d > maxf || d > rem) continue;
		if (rem % d != 0) continue;
		long double nc = cur + (long double)(d - 1) * A355919_lq[idx];
		if (nc >= *best) continue;
		A355919_dfs(divs, nd, rem / d, d, idx + 1, nc, best, found);
	}
}

static int A355919_cmp(const void *x, const void *y)
{
	uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
	return a1 < b1 ? -1 : (a1 > b1 ? 1 : 0);
}

static bool A355919_test(uint64_t m)
{
	uint64_t fac[64];
	int nf = 0;
	uint64_t t = m;
	uint64_t pr[64];
	int ex[64];
	int np = 0;
	for (uint64_t p = 2; p * p <= t; p++) {
		if (t % p == 0) {
			int e = 0;
			while (t % p == 0) { t /= p; e++; fac[nf++] = p; }
			pr[np] = p; ex[np] = e; np++;
		}
	}
	if (t > 1) { fac[nf++] = t; pr[np] = t; ex[np] = 1; np++; }
	/* factors are in ascending order; b uses descending */
	long double blog = 0;
	for (int i = 0; i < nf; i++)
		blog += (long double)(fac[nf - 1 - i] - 1) * A355919_lq[i];

	/* divisors */
	size_t cap = 1;
	for (int i = 0; i < np; i++) cap *= (size_t)(ex[i] + 1);
	uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
	size_t nd = 1;
	divs[0] = 1;
	for (int i = 0; i < np; i++) {
		size_t cur = nd;
		uint64_t pw = 1;
		for (int e = 1; e <= ex[i]; e++) {
			pw *= pr[i];
			for (size_t j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
		}
	}
	qsort(divs, nd, sizeof(uint64_t), A355919_cmp);

	long double best = blog - 1e-9L;
	bool found = false;
	A355919_dfs(divs, (int)nd, m, m, 0, 0.0L, &best, &found);
	free(divs);
	return found;
}

static uint64_t *A355919_terms = NULL;
static size_t A355919_cnt = 0, A355919_capc = 0;
static uint64_t A355919_next = 2;

seq_t a(idx_t n)
{
	if (!A355919_ready) A355919_init();
	while (A355919_cnt < (size_t)n) {
		uint64_t m = A355919_next++;
		if (A355919_test(m)) {
			if (A355919_cnt >= A355919_capc) {
				A355919_capc = A355919_capc ? A355919_capc * 2 : 64;
				A355919_terms = (uint64_t *)realloc(A355919_terms, A355919_capc * sizeof(uint64_t));
			}
			A355919_terms[A355919_cnt++] = m;
		}
	}
	return (seq_t)A355919_terms[n - 1];
}
