/*
 * A244275  Year of state admission into the United States sorted by years.
 * https://oeis.org/A244275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A244275: a(n) = year of the n-th state admitted to the U.S. (states sorted by admission date). */
/* 50-state finite list.  a(1)=1787, a(2)=1787, a(3)=1787, …, a(50)=1959. */
#include <stddef.h>

static const seq_t _admission_years[50] = {
    /*  1 */ 1787, /* Delaware */
    /*  2 */ 1787, /* Pennsylvania */
    /*  3 */ 1787, /* New Jersey */
    /*  4 */ 1788, /* Georgia */
    /*  5 */ 1788, /* Connecticut */
    /*  6 */ 1788, /* Massachusetts */
    /*  7 */ 1788, /* Maryland */
    /*  8 */ 1788, /* South Carolina */
    /*  9 */ 1788, /* New Hampshire */
    /* 10 */ 1788, /* Virginia */
    /* 11 */ 1788, /* New York */
    /* 12 */ 1789, /* North Carolina */
    /* 13 */ 1790, /* Rhode Island */
    /* 14 */ 1791, /* Vermont */
    /* 15 */ 1792, /* Kentucky */
    /* 16 */ 1796, /* Tennessee */
    /* 17 */ 1803, /* Ohio */
    /* 18 */ 1812, /* Louisiana */
    /* 19 */ 1816, /* Indiana */
    /* 20 */ 1817, /* Mississippi */
    /* 21 */ 1818, /* Alabama */
    /* 22 */ 1819, /* Illinois */
    /* 23 */ 1820, /* Maine */
    /* 24 */ 1821, /* Arkansas */
    /* 25 */ 1836, /* Michigan */
    /* 26 */ 1837, /* Florida */
    /* 27 */ 1845, /* Texas */
    /* 28 */ 1845, /* Texas (duplicate for the same year, order preserved as in admission dates) */
    /* 29 */ 1846, /* Iowa */
    /* 30 */ 1848, /* Wisconsin */
    /* 31 */ 1850, /* California */
    /* 32 */ 1858, /* Minnesota */
    /* 33 */ 1859, /* Oregon */
    /* 34 */ 1861, /* Kansas */
    /* 35 */ 1863, /* West Virginia */
    /* 36 */ 1864, /* Nevada */
    /* 37 */ 1867, /* Nebraska */
    /* 38 */ 1876, /* Colorado */
    /* 39 */ 1889, /* North Dakota */
    /* 40 */ 1889, /* South Dakota */
    /* 41 */ 1889, /* Montana */
    /* 42 */ 1889, /* Washington */
    /* 43 */ 1890, /* Idaho */
    /* 44 */ 1890, /* Wyoming */
    /* 45 */ 1896, /* Utah */
    /* 46 */ 1907, /* Oklahoma */
    /* 47 */ 1912, /* New Mexico */
    /* 48 */ 1912, /* Arizona */
    /* 49 */ 1959, /* Alaska */
    /* 50 */ 1959  /* Hawaii */
};

seq_t a(idx_t n)
{
    if (n == 0 || n > 50)
        return 0;
    return _admission_years[n - 1];
}
