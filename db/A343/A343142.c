/*
 * A343142  a(n) = sqrt(k*A001414(k)+1) for k in A343141.
 * https://oeis.org/A343142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(k*A001414(k)+1) for k = A343141(n), the numbers k
 *            with k*sopfr(k)+1 = p^2, p prime. Computed by a search over k <=
 *            K (K grows by x4), split by q = largest prime factor of k (k =
 *            q*j, t = sopfr(j)): - q > Q: q | p-1 or q | p+1, so p = q*r + e
 *            (e = +-1), which gives q*(r^2 - j) = j*t -...
 * Range:     u8 n <= 15, u16 n <= 107, u32 n <= 450, u64 n <= 450,
 *            u128 n <= 450, big any n
 * Validated: standard, pass, 70 terms through n = 446 (2026-11-10)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

typedef struct { uint64_t k, p; } A343142_pair;

static A343142_pair *A343142_buf = 0;
static size_t A343142_bn = 0, A343142_bcap = 0;
static uint64_t *A343142_res = 0;
static size_t A343142_rn = 0;
static uint64_t A343142_K = 0;

static uint64_t A343142_m, A343142_Sm, A343142_smax, A343142_Kcur, A343142_pcur;
static uint64_t A343142_pr[64];
static int A343142_ex[64];
static int A343142_pc;

static void A343142_push(uint64_t k, uint64_t p)
{
	if (A343142_bn == A343142_bcap) {
		size_t nc = A343142_bcap ? A343142_bcap * 2 : 1024;
		A343142_pair *t = (A343142_pair *)realloc(A343142_buf, nc * sizeof(A343142_pair));
		if (!t) return;
		A343142_buf = t;
		A343142_bcap = nc;
	}
	A343142_buf[A343142_bn].k = k;
	A343142_buf[A343142_bn].p = p;
	A343142_bn++;
}

static int A343142_cmp(const void *x, const void *y)
{
	const A343142_pair *a1 = (const A343142_pair *)x;
	const A343142_pair *b1 = (const A343142_pair *)y;
	if (a1->k < b1->k) return -1;
	if (a1->k > b1->k) return 1;
	return 0;
}

static int A343142_addfac(const uint16_t *spf, uint64_t x, int cnt)
{
	while (x > 1) {
		uint64_t f = spf[x];
		int e = 0, i;
		if (f == 0) f = x;
		while (x % f == 0) { x /= f; e++; }
		for (i = 0; i < cnt; i++) if (A343142_pr[i] == f) break;
		if (i == cnt) { A343142_pr[cnt] = f; A343142_ex[cnt] = 0; cnt++; }
		A343142_ex[i] += e;
	}
	return cnt;
}

static void A343142_dfs(int i, uint64_t s, uint64_t sop)
{
	uint64_t pr, ss, so;
	int e;
	if (i == A343142_pc) {
		if (s >= 2 && A343142_Sm - sop == s) {
			uint64_t k = A343142_m / s;
			if (k <= A343142_Kcur) A343142_push(k, A343142_pcur);
		}
		return;
	}
	pr = A343142_pr[i];
	ss = s;
	so = sop;
	for (e = 0; e <= A343142_ex[i]; e++) {
		A343142_dfs(i + 1, ss, so);
		if (e == A343142_ex[i] || ss > A343142_smax / pr) break;
		ss *= pr;
		so += pr;
	}
}

static int A343142_bitlen(uint64_t x)
{
	int b = 0;
	while (x) { b++; x >>= 1; }
	return b;
}

static void A343142_build(uint64_t K)
{
	uint64_t Q, smax, Pmax, J, N, i, jj, p, j;
	uint16_t *spf;
	size_t w, r0;

	Q = isqrt_u64(K) / 2;
	if (Q < 64) Q = 64;
	if (Q > 65521) Q = 65521;
	smax = Q * (uint64_t)A343142_bitlen(K) / (uint64_t)(A343142_bitlen(Q) - 1) + 1;
	Pmax = isqrt_u64(K * smax + 1) + 1;
	J = K / (Q + 1);
	N = (Pmax + 1 > J ? Pmax + 1 : J) + 1;

	spf = (uint16_t *)calloc((size_t)N + 1, sizeof(uint16_t));
	if (!spf) return;
	for (i = 2; i * i <= N; i++) {
		if (spf[i]) continue;
		for (jj = i * i; jj <= N; jj += i)
			if (!spf[jj]) spf[jj] = (uint16_t)i;
	}

	A343142_bn = 0;
	A343142_Kcur = K;
	A343142_smax = smax;

	/* Part S: largest prime factor of k <= Q */
	for (p = 3; p <= Pmax; p++) {
		int c;
		if (spf[p]) continue;
		A343142_m = p * p - 1;
		A343142_pcur = p;
		c = A343142_addfac(spf, p - 1, 0);
		c = A343142_addfac(spf, p + 1, c);
		A343142_pc = c;
		A343142_Sm = 0;
		for (i = 0; i < (uint64_t)c; i++)
			A343142_Sm += A343142_pr[i] * (uint64_t)A343142_ex[i];
		A343142_dfs(0, 1, 0);
	}

	/* Part L: largest prime factor q of k > Q, k = q*j */
	for (j = 1; j <= J; j++) {
		uint64_t x = j, t = 0, P = 1, qmin, r, jt;
		while (x > 1) {
			uint64_t f = spf[x];
			if (f == 0) f = x;
			t += f;
			if (f > P) P = f;
			x /= f;
		}
		qmin = (Q + 1 > P) ? Q + 1 : P;
		jt = j * t;
		for (r = isqrt_u64(j) + 1; ; r++) {
			uint64_t d = r * r - j;
			int e;
			if (jt + 2 * r < qmin * d) break;
			for (e = 0; e < 2; e++) {
				uint64_t num, q, pp;
				if (e == 0) {
					if (jt <= 2 * r) continue;
					num = jt - 2 * r;
				} else {
					num = jt + 2 * r;
				}
				if (num % d) continue;
				q = num / d;
				if (q < qmin || q > K / j) continue;
				if (!is_prime_u64(q)) continue;
				pp = (e == 0) ? q * r + 1 : q * r - 1;
				if (pp < 2) continue;
				if (is_prime_u64(pp)) A343142_push(q * j, pp);
			}
		}
	}
	free(spf);

	if (A343142_bn > 0)
		qsort(A343142_buf, A343142_bn, sizeof(A343142_pair), A343142_cmp);
	free(A343142_res);
	A343142_res = (uint64_t *)malloc((A343142_bn + 1) * sizeof(uint64_t));
	if (!A343142_res) { A343142_rn = 0; return; }
	w = 0;
	for (r0 = 0; r0 < A343142_bn; r0++) {
		if (r0 > 0 && A343142_buf[r0].k == A343142_buf[r0 - 1].k) continue;
		A343142_res[w++] = A343142_buf[r0].p;
	}
	A343142_rn = w;
	A343142_K = K;
}

seq_t a(idx_t n)
{
	while ((uint64_t)A343142_rn < (uint64_t)n) {
		uint64_t K = A343142_K ? A343142_K * 4 : ((uint64_t)1 << 24);
		if (K > ((uint64_t)1 << 38)) break;
		A343142_build(K);
		if (A343142_K != K) break;
	}
	if ((uint64_t)A343142_rn < (uint64_t)n) return (seq_t)0;
	return (seq_t)A343142_res[n - 1];
}
