/*
 * A347079  a(n) is the least prime that is the perimeter of exactly n
 *          isosceles triangles with prime sides.
 * https://oeis.org/A347079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that there are exactly n * pairs of
 *            primes (q,r) with 2*q + r = p and r < 2*q
 * Range:     u8 n <= 6, u16 n <= 238, u32 n <= 808, u64 n <= 808,
 *            u128 n <= 808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Forward declaration from the build. */
extern seq_t a(idx_t n);

/* ----------  Static state ---------- */
static idx_t cur_limit = 0;               /* current q‑bound used for sieve   */
static idx_t cur_maxp  = 0;               /* current maximum prime value      */
static unsigned char *is_prime   = NULL;  /* 0/1 array up to cur_maxp         */
static uint16_t      *cnt        = NULL;  /* count of pairs for each prime    */
static uint64_t      *prime_list = NULL;  /* list of all primes up to cur_maxp*/
static size_t        prime_cnt  = 0;      /* number of primes in prime_list   */
static seq_t        *answer_arr = NULL;   /* answer_arr[k] = least prime with k pairs, 0 if unknown */
static size_t        ans_cap    = 0;      /* allocated size of answer_arr      */

/* ----------  Utility functions ---------- */
static void free_all(void)
{
    free(is_prime);
    free(cnt);
    free(prime_list);
}

static void sieve_and_prime_list(idx_t limit)
{
    /* limit = cur_maxp */
    is_prime = (unsigned char*)malloc((size_t)(limit + 1));
    if (!is_prime) return;
    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;

    /* Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    /* build prime list */
    size_t est = limit / 10 + 10;                /* rough estimate */
    prime_list = (uint64_t*)malloc(est * sizeof(uint64_t));
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            if (prime_cnt == est) {
                est *= 2;
                prime_list = (uint64_t*)realloc(prime_list, est * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = (uint64_t)i;
        }
    }
}

/* Rebuild all cached data for a new q‑bound `qlim`. */
static void rebuild(idx_t qlim)
{
    free_all();

    cur_limit = qlim;
    cur_maxp  = 4 * qlim;              /* maximal possible p = 2q + r <= 4q */
    sieve_and_prime_list(cur_maxp);

    /* Allocate count array. */
    cnt = (uint16_t*)calloc((size_t)(cur_maxp + 1), sizeof(uint16_t));
    if (!cnt) return;

    /* Count pairs (q,r). */
    for (size_t iq = 0; iq < prime_cnt && prime_list[iq] <= qlim; ++iq) {
        uint64_t q = prime_list[iq];
        for (size_t ir = 0; ir < prime_cnt && prime_list[ir] < 2 * q; ++ir) {
            uint64_t r = prime_list[ir];
            uint64_t p = 2 * q + r;
            if (p > cur_maxp) break;
            if (is_prime[(size_t)p]) {
                if (cnt[(size_t)p] < 65535) cnt[(size_t)p]++;
            }
        }
    }

    /* Determine maximal count value. */
    size_t maxcnt = 0;
    for (size_t i = 3; i <= (size_t)cur_maxp; ++i) {
        if (is_prime[i]) {
            size_t c = cnt[i];
            if (c > maxcnt) maxcnt = c;
        }
    }

    /* Allocate or enlarge answer array. */
    size_t new_cap = maxcnt + 1;
    if (ans_cap < new_cap) {
        answer_arr = (seq_t*)realloc(answer_arr, new_cap * sizeof(seq_t));
        memset(answer_arr, 0, new_cap * sizeof(seq_t));
        ans_cap = new_cap;
    } else {
        memset(answer_arr, 0, ans_cap * sizeof(seq_t));
    }

    /* Fill answer array with the smallest prime for each count. */
    for (size_t i = 3; i <= (size_t)cur_maxp; ++i) {
        if (!is_prime[i]) continue;
        size_t c = cnt[i];
        if (c == 0) continue;
        if (answer_arr[c] == 0) answer_arr[c] = (seq_t)i;
    }
    /* n = 0 case */
    answer_arr[0] = (seq_t)2;
}

/* Ensure that answer_arr[n] is filled; expand limit until found. */
static void ensure_answer(idx_t n)
{
    if (n == 0) {
        if (answer_arr == NULL || ans_cap == 0) {
            answer_arr = (seq_t*)malloc(sizeof(seq_t));
            answer_arr[0] = (seq_t)2;
            ans_cap = 1;
        }
        return;
    }

    /* If already available, nothing else to do. */
    if (answer_arr != NULL && (size_t)n < ans_cap && answer_arr[n] != 0)
        return;

    /* Start with a modest bound and double until we locate n. */
    idx_t lim = (cur_limit > 0) ? cur_limit * 2 : 20;
    while (true) {
        rebuild(lim);
        if (answer_arr != NULL && (size_t)n < ans_cap && answer_arr[n] != 0)
            return;
        lim *= 2;
    }
}

/* ----------  Requested function ---------- */
seq_t a(idx_t n)
{
    ensure_answer(n);
    return answer_arr[n];
}
