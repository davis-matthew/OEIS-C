/*
 * A337919  Inverse permutation to A337918.
 * https://oeis.org/A337919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = m such that A337918(m) = n, where A337918 is the
 *            lexicographically earliest permutation with A002024(m) as a
 *            substring of A337918(m). Computed by simulating row k = 1, 2,
 *            ... of A002024, each taking its k smallest unused numbers
 *            containing k as a substring, until n is claimed
 * Range:     u8 n <= 29, u16 n <= 3999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8674 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *T_key = 0;
static uint64_t *T_val = 0;
static size_t T_cap = 0;
static size_t T_cnt = 0;
static uint64_t T_nextm = 1;
static uint64_t T_rows = 0;

static size_t T_hash(uint64_t x, size_t cap) {
  return (size_t)(((x * 0x9E3779B97F4A7C15ULL) >> 20) & (uint64_t)(cap - 1));
}

static uint64_t T_find(uint64_t key) {
  if (!T_cap) return 0;
  size_t h = T_hash(key, T_cap);
  while (T_key[h]) {
    if (T_key[h] == key) return T_val[h];
    h = (h + 1) & (T_cap - 1);
  }
  return 0;
}

static void T_insert_raw(uint64_t key, uint64_t val) {
  size_t h = T_hash(key, T_cap);
  while (T_key[h]) h = (h + 1) & (T_cap - 1);
  T_key[h] = key;
  T_val[h] = val;
}

static void T_insert(uint64_t key, uint64_t val) {
  if ((T_cnt + 1) * 2 > T_cap) {
    size_t ncap = T_cap ? T_cap * 2 : 1024;
    uint64_t *ok = T_key, *ov = T_val;
    size_t oc = T_cap;
    T_key = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    T_val = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    T_cap = ncap;
    for (size_t i = 0; i < oc; i++)
      if (ok[i]) T_insert_raw(ok[i], ov[i]);
    free(ok);
    free(ov);
  }
  T_insert_raw(key, val);
  T_cnt++;
}

static int T_cmp(const void *a, const void *b) {
  uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
  return (x > y) - (x < y);
}

static uint64_t T_p10(int e) {
  uint64_t r = 1;
  while (e-- > 0) r *= 10;
  return r;
}

static void T_do_row(uint64_t k) {
  int d = 0;
  for (uint64_t t = k; t; t /= 10) d++;
  uint64_t need = k;
  for (int L = d; need > 0 && L <= 18; L++) {
    size_t cap = 0, n = 0;
    uint64_t *v = 0;
    for (int p = 0; p <= L - d; p++) {
      int s = L - p - d;
      uint64_t plo = p ? T_p10(p - 1) : 0;
      uint64_t phi = p ? T_p10(p) : 1;
      uint64_t sn = T_p10(s);
      uint64_t mid = k * sn;
      uint64_t sh = T_p10(L - p);
      for (uint64_t pr = plo; pr < phi; pr++) {
        for (uint64_t su = 0; su < sn; su++) {
          if (n == cap) {
            cap = cap ? cap * 2 : 256;
            v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
          }
          v[n++] = pr * sh + mid + su;
        }
      }
    }
    qsort(v, n, sizeof(uint64_t), T_cmp);
    for (size_t i = 0; i < n && need > 0; i++) {
      if (i > 0 && v[i] == v[i - 1]) continue;
      if (T_find(v[i])) continue;
      T_insert(v[i], T_nextm++);
      need--;
    }
    free(v);
  }
}

seq_t a(idx_t n) {
  uint64_t key = (uint64_t)n;
  uint64_t r = T_find(key);
  while (!r) {
    T_rows++;
    T_do_row(T_rows);
    r = T_find(key);
  }
  return (seq_t)r;
}
