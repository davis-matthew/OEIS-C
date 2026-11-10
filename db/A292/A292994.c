/*
 * A292994  Number of integers m, 1 <= m <= A002569(n), that are not terms in
 *          the triangle T(n,k) of A008284.
 * https://oeis.org/A292994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002569(n) - #{distinct values among T(r,k), 1<=k<=r<=n}
 *            (A008284), computed by DP + sort/unique
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static void A292994_sift(seq_t *v, size_t start, size_t end)
{
	size_t root = start;
	while (2 * root + 1 < end) {
		size_t child = 2 * root + 1;
		if (child + 1 < end && v[child] < v[child + 1]) child++;
		if (v[root] < v[child]) {
			seq_t t = v[root]; v[root] = v[child]; v[child] = t;
			root = child;
		} else return;
	}
}

static void A292994_heapsort(seq_t *v, size_t n)
{
	size_t i;
	if (n < 2) return;
	for (i = n / 2; i-- > 0;) A292994_sift(v, i, n);
	for (i = n - 1; i > 0; i--) {
		seq_t t = v[0]; v[0] = v[i]; v[i] = t;
		A292994_sift(v, 0, i);
	}
}

seq_t a(idx_t n)
{
	long long N = (long long)n;
	long long m, k;
	size_t total, cnt, i;
	seq_t *f, *vals;
	seq_t best, distinct, res;

	if (N <= 0) return 0;

	total = (size_t)N * (size_t)(N + 1) / 2;
	f = mem_new(seq_t, (size_t)(N + 1));
	vals = mem_new(seq_t, total);
	if (!f || !vals) {
		if (f) mem_free(f);
		if (vals) mem_free(vals);
		return 0;
	}
	for (m = 0; m <= N; m++) f[m] = 0;
	f[0] = 1;

	best = 0;
	cnt = 0;
	for (k = 1; k <= N; k++) {
		for (m = k; m <= N - k; m++)
			f[m] += f[m - k];
		for (m = 0; m <= N - k; m++)
			vals[cnt++] = f[m];
		if (f[N - k] > best) best = f[N - k];
	}

	A292994_heapsort(vals, cnt);
	distinct = 0;
	for (i = 0; i < cnt; i++) {
		if (i == 0 || vals[i] != vals[i - 1]) distinct++;
	}

	res = best - distinct;
	mem_free(f);
	mem_free(vals);
	return res;
}
