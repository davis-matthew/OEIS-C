/*
 * A300813  a(n) is the smallest multiple of A001414(a(n-1)) not yet seen in
 *          the sequence; a(1)=2.
 * https://oeis.org/A300813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1)=2; a(n) = least multiple of A001414(a(n-1)) not already in
 *            the sequence (simulation with a hash set)
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9034 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct { uint64_t *k; uint64_t *v; size_t cap, cnt; } A300813_ht;

static size_t A300813_hash(uint64_t x, size_t cap)
{
	x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33;
	return (size_t)(x & (cap - 1));
}

static void A300813_init(A300813_ht *h, size_t cap)
{
	h->cap = cap; h->cnt = 0;
	h->k = (uint64_t *)calloc(cap, sizeof(uint64_t));
	h->v = (uint64_t *)calloc(cap, sizeof(uint64_t));
}

static uint64_t *A300813_slot(A300813_ht *h, uint64_t key)
{
	size_t i = A300813_hash(key, h->cap);
	while (h->k[i] != 0 && h->k[i] != key) i = (i + 1) & (h->cap - 1);
	return &h->k[i];
}

static void A300813_put(A300813_ht *h, uint64_t key, uint64_t val)
{
	if ((h->cnt + 1) * 2 > h->cap) {
		A300813_ht n;
		A300813_init(&n, h->cap * 2);
		for (size_t i = 0; i < h->cap; i++)
			if (h->k[i]) {
				uint64_t *s = A300813_slot(&n, h->k[i]);
				size_t idx = (size_t)(s - n.k);
				n.k[idx] = h->k[i]; n.v[idx] = h->v[i]; n.cnt++;
			}
		free(h->k); free(h->v);
		*h = n;
	}
	uint64_t *s = A300813_slot(h, key);
	size_t idx = (size_t)(s - h->k);
	if (h->k[idx] == 0) { h->k[idx] = key; h->cnt++; }
	h->v[idx] = val;
}

static bool A300813_has(A300813_ht *h, uint64_t key)
{
	return *A300813_slot(h, key) == key;
}

static uint64_t A300813_get(A300813_ht *h, uint64_t key)
{
	uint64_t *s = A300813_slot(h, key);
	if (*s != key) return 0;
	return h->v[s - h->k];
}

static uint64_t A300813_sopfr(uint64_t n)
{
	uint64_t r = 0;
	while (n % 2 == 0) { n /= 2; r += 2; }
	for (uint64_t p = 3; n / p >= p; p += 2)
		while (n % p == 0) { n /= p; r += p; }
	if (n > 1) r += n;
	return r;
}

static uint64_t *A300813_terms = NULL;
static size_t A300813_len = 0, A300813_capt = 0;
static A300813_ht A300813_seen, A300813_ptr;
static bool A300813_ready = false;

seq_t a(idx_t n)
{
	if (!A300813_ready) {
		A300813_init(&A300813_seen, 1024);
		A300813_init(&A300813_ptr, 1024);
		A300813_capt = 1024;
		A300813_terms = (uint64_t *)malloc(A300813_capt * sizeof(uint64_t));
		A300813_terms[0] = 2;
		A300813_len = 1;
		A300813_put(&A300813_seen, 2, 1);
		A300813_ready = true;
	}
	while (A300813_len < (size_t)n) {
		uint64_t m = A300813_sopfr(A300813_terms[A300813_len - 1]);
		uint64_t k = A300813_get(&A300813_ptr, m);
		if (k == 0) k = 1;
		while (A300813_has(&A300813_seen, m * k)) k++;
		uint64_t v = m * k;
		A300813_put(&A300813_ptr, m, k);
		A300813_put(&A300813_seen, v, 1);
		if (A300813_len == A300813_capt) {
			A300813_capt *= 2;
			A300813_terms = (uint64_t *)realloc(A300813_terms, A300813_capt * sizeof(uint64_t));
		}
		A300813_terms[A300813_len++] = v;
	}
	return (seq_t)A300813_terms[n - 1];
}
