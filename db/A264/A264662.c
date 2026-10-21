/*
 * A264662  Triangle read by rows: row n contains the first n primes in
 *          lexicographical order of their mirrored binary representation.
 * https://oeis.org/A264662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1486, u16 n <= 9332, u32 n <= 9332, u64 n <= 9332,
 *            u128 n <= 9332, big any n
 * Helpers:   compute_rev_and_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A264662
 *  Triangle: row n contains the first n primes sorted lexicographically
 *  by the mirrored binary representation of each prime.
 *  a(n) is the n-th element read row‑by‑row.
 *
 *  The function builds the required row, sorts the primes by the
 *  reversed binary bits, and returns the k‑th element of that row.
 *
 *  No external libraries beyond the standard headers or the helper
 *  functions listed in the problem statement are used.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_rev_and_len.h"

/* ---------- Prime storage (dynamic array) ---------- */
static uint32_t *prime_list   = NULL;
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;

/* Ensure that prime_list contains at least "need" primes. */
static void ensure_primes_upto(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Grow the array if necessary. */
    if (prime_cap < need) {
        size_t newcap = prime_cap ? prime_cap * 2 : 16;
        if (newcap < need)
            newcap = need;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, newcap * sizeof(uint32_t));
        if (!tmp)
            exit(1);        /* fatal error: allocation failed */
        prime_list = tmp;
        prime_cap  = newcap;
    }

    /* Generate missing primes starting from the last known prime. */
    if (prime_cnt == 0) {
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    uint32_t cand = prime_list[prime_cnt - 1] + 1;
    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= cand; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = cand;
            cand = prime_list[prime_cnt - 1] + 1;
        } else {
            ++cand;
        }
    }
}

/* ---------- Data for sorting ---------- */
typedef struct {
    uint32_t prime;   /* the prime itself */
    uint32_t rev;     /* reversed binary bits */
    uint8_t  len;     /* length of the binary representation */
} PrimeInfo;

/* Comparator for qsort: lexicographic order of reversed binary strings. */
static int compare_rev(const void *a, const void *b)
{
    const PrimeInfo *pa = (const PrimeInfo *)a;
    const PrimeInfo *pb = (const PrimeInfo *)b;
    uint8_t L1 = pa->len;
    uint8_t L2 = pb->len;
    uint32_t rev1 = pa->rev;
    uint32_t rev2 = pb->rev;

    uint8_t minL = L1 < L2 ? L1 : L2;
    for (uint8_t i = 0; i < minL; ++i) {
        uint32_t mask1 = 1U << (L1 - 1 - i);
        uint32_t mask2 = 1U << (L2 - 1 - i);
        int b1 = (rev1 & mask1) != 0;
        int b2 = (rev2 & mask2) != 0;
        if (b1 != b2)
            return b1 < b2 ? -1 : 1;
    }
    /* If all shared bits are equal, shorter string is lexicographically smaller */
    if (L1 != L2)
        return L1 < L2 ? -1 : 1;
    return 0;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n==0 */

    /* Determine the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;               /* 1‑based column in the row */

    /* Ensure we have at least r primes. */
    ensure_primes_upto((size_t)r);

    /* Build an array of PrimeInfo for the first r primes. */
    PrimeInfo *arr = (PrimeInfo *)malloc(r * sizeof(PrimeInfo));
    if (!arr)
        exit(1);                            /* fatal error */

    for (idx_t i = 0; i < r; ++i) {
        arr[i].prime = prime_list[(size_t)i];
        compute_rev_and_len(arr[i].prime, &arr[i].rev, &arr[i].len);
    }

    /* Sort by reversed binary representation lex order. */
    qsort(arr, r, sizeof(PrimeInfo), compare_rev);

    seq_t result = (seq_t)arr[k - 1].prime;  /* k is 1‑based */

    free(arr);
    return result;
}
