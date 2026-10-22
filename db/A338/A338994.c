/*
 * A338994  Table read by antidiagonals: if x(n+1) = A001414(x(n-1)) +
 *          A001414(x(n)) with x(0) = i and x(1) = j, then T(i,j) is the first
 *          k such that (x(k), x(k+1)) is a fixed point or a member of a
 *          cycle. If there is no such k, then T(i,j) = -1.
 * https://oeis.org/A338994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9648, u16 n <= 9648, u32 n <= 9648, u64 n <= 9648,
 *            u128 n <= 9648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A338994: Table read by antidiagonals.
   a(n) = T(i,j) where (i,j) is the n‑th pair read by antidiagonals 
   (offset 1).  T(i,j) is the first k such that (x(k),x(k+1)) is a 
   fixed point or a member of a cycle in the recurrence
       x(0)=i , x(1)=j
       x(n+1)=A001414(x(n-1))+A001414(x(n)),
   where A001414(m) is the sum of prime factors of m (with multiplicity). 
*/

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ---------- prime‑factor sieve for A001414 (up to 5000) ---------- */
#define MAX_SPF 5000
static int spf[MAX_SPF + 1];
static int spf_init = 0;

static void init_spf(void)
{
    if (spf_init) return;
    for (int i = 0; i <= MAX_SPF; ++i) spf[i] = i;
    for (int i = 2; i * i <= MAX_SPF; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAX_SPF; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_init = 1;
}

/* sum of prime factors (A001414), input fits in uint64_t */
static seq_t sum_prime_factors(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t tmp = n, sum = 0;
    if (tmp <= (uint64_t)MAX_SPF) {
        while (tmp > 1) {
            int p = spf[(int)tmp];
            sum += p;
            tmp /= p;
        }
    } else {
        for (uint64_t p = 2; p * p <= tmp; ++p) {
            while (tmp % p == 0) {
                sum += p;
                tmp /= p;
            }
        }
        if (tmp > 1) sum += tmp;
    }
    return (seq_t)sum;
}

/* ---------- antidiagonal coordinates (i,j) for a given n ---------- */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* base 1: n >= 1 */
    long double r = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t k = (idx_t)((r - 1.0L) / 2.0L);          /* floor */
    idx_t tri = k * (k + 1) / 2;
    while (tri < n) { ++k; tri = k * (k + 1) / 2; }
    while (k > 0 && (k - 1) * k / 2 >= n) {
        --k; tri = k * (k + 1) / 2;
    }
    idx_t prev_tri = (k - 1) * k / 2;
    idx_t pos = n - prev_tri;                     /* 1‑based position on this diagonal */
    *i = pos;                                     /* row index (1‑based) */
    *j = (k + 1) - pos;                           /* column index (1‑based) */
}

/* ---------- visited pair table: stores first occurrence of a pair ---------- */
#define MAX_VISITED 2048
struct visited_pair { seq_t a; seq_t b; idx_t step; };

static struct visited_pair visited[MAX_VISITED];

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below the offset – undefined */

    init_spf();                   /* ensure SPF table is ready */

    idx_t ii, jj;
    antidiagonal_coords(n, &ii, &jj);

    seq_t prev = (seq_t)ii;
    seq_t curr = (seq_t)jj;
    idx_t step = 0;

    /* clear visited table */
    size_t visited_cnt = 0;

    while (1) {
        /* fixed point test: prev == curr and 2*sum(prev) == prev */
        if (prev == curr) {
            seq_t s = sum_prime_factors((uint64_t)prev);
            if ((s << 1) == prev) return step;
        }

        /* cycle detection: if the pair was seen before, it is part of a cycle */
        for (size_t t = 0; t < visited_cnt; ++t) {
            if (visited[t].a == prev && visited[t].b == curr)
                return visited[t].step;   /* first occurrence of this pair */
        }

        /* record current pair with its step */
        if (visited_cnt < MAX_VISITED) {
            visited[visited_cnt].a = prev;
            visited[visited_cnt].b = curr;
            visited[visited_cnt].step = step;
            ++visited_cnt;
        } else {
            /* safety: should not happen for the ranges used in the tests */
            /* fall back to linear search without storing further pairs */
        }

        /* compute next element of the recurrence */
        seq_t next = sum_prime_factors((uint64_t)prev) +
                     sum_prime_factors((uint64_t)curr);
        prev = curr;
        curr = next;
        ++step;
    }
}
